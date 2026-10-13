# Checks the data of the mod against the sources, and the zips of dist\ if they exist. Run by
# Compile.ps1 after a build (not with -NoPackage); it can also be run alone:
#   powershell -NoProfile -ExecutionPolicy Bypass -File .\Check.ps1
#
# - every message id of the code (defs.h) is in every inventory_filter*.msg of InventoryFilter.dat\text;
# - every INI key the code reads is in InvenFilter.ini, and the default given to IniInt is the value
#   that the shipped INI has;
# - every comment block of the INI is in the dictionary, in every language (TranslateIni.ps1);
# - the zips of dist\ have entries named with "/" and hold the compiled script.
# Prints what is wrong and ends with an error if anything is.

param(
    [string] $Root
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if (-not $Root) { $Root = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path } }
$source = Join-Path $Root 'source'
$dat = Join-Path $Root 'InventoryFilter.dat'
$problems = [System.Collections.Generic.List[string]]::new()
function Fail([string] $message) { $script:problems.Add($message) }

$utf8 = New-Object System.Text.UTF8Encoding($false)
$sources = Get-ChildItem -LiteralPath $source -Include '*.ssl' -File -Recurse
$sourceText = ($sources | ForEach-Object { [System.IO.File]::ReadAllText($_.FullName, $utf8) }) -join "`n"
$defs = [System.IO.File]::ReadAllText((Join-Path $source 'defs.h'), $utf8)

# ---- message ids
$required = [System.Collections.Generic.SortedSet[int]]::new()
foreach ($m in [regex]::Matches($defs, '(?m)^#define\s+(MSG_[A-Z_0-9]+)\s+(\d+)\b')) {
    if ($m.Groups[1].Value -match '^MSG_(WIN|TEXT)_') { continue }
    [void]$required.Add([int]$m.Groups[2].Value)
}
$filterCount = [regex]::Match($defs, '(?m)^#define\s+FILTER_COUNT\s+(\d+)').Groups[1].Value
$sortCount = [regex]::Match($defs, '(?m)^#define\s+SORT_COUNT\s+(\d+)').Groups[1].Value
$tipBase = [regex]::Match($defs, '(?m)^#define\s+MSG_FILTER_TIP\s+(\d+)').Groups[1].Value
$sortBase = [regex]::Match($defs, '(?m)^#define\s+MSG_SORT_TIP\s+(\d+)').Groups[1].Value
if ($filterCount -and $tipBase) { foreach ($n in 0..([int]$filterCount - 1)) { [void]$required.Add([int]$tipBase + $n) } }
if ($sortCount -and $sortBase) { foreach ($n in 0..([int]$sortCount - 1)) { [void]$required.Add([int]$sortBase + $n) } }

$msgFiles = Get-ChildItem -LiteralPath (Join-Path $dat 'text') -Filter 'inventory_filter*.msg' -File -Recurse
foreach ($file in $msgFiles) {
    # Any single-byte code page reads the ids and the braces the same way.
    $content = [System.Text.Encoding]::GetEncoding(28591).GetString([System.IO.File]::ReadAllBytes($file.FullName))
    foreach ($id in $required) {
        if ($content -notmatch ('(?m)^\s*\{' + $id + '\}\{[^}]*\}\{')) {
            Fail ("message {0} missing in {1}" -f $id, $file.FullName.Substring($Root.Length + 1))
        }
    }
}
if (@($msgFiles).Count -eq 0) { Fail 'no inventory_filter*.msg found' }

# ---- INI keys
$iniPath = Join-Path $dat 'InvenFilter.ini'
$iniLines = [System.IO.File]::ReadAllLines($iniPath, $utf8)
$iniValues = @{}
$section = ''
foreach ($line in $iniLines) {
    if ($line -match '^\[(.+)\]\s*$') { $section = $Matches[1]; continue }
    if ($line -match '^(\w+)\s*=(.*)$') { $iniValues["$section|$($Matches[1])"] = $Matches[2].Trim() }
    elseif ($line -match '^;(Language)=') { $iniValues["$section|$($Matches[1])"] = '' }
}
$read = @{}
foreach ($m in [regex]::Matches($sourceText, 'Ini(?:Get|GetString|Int)\("(\w+)"(?:,\s*([^)]+?))?\)')) {
    $key = $m.Groups[1].Value
    if (-not $read.ContainsKey($key)) { $read[$key] = $null }
    if ($m.Groups[2].Success) { $read[$key] = $m.Groups[2].Value.Trim() }
}
foreach ($key in ($read.Keys | Sort-Object)) {
    if (-not $iniValues.ContainsKey("InventoryFilter|$key")) { Fail "INI key $key is read by the code and not in InvenFilter.ini"; continue }
    $default = $read[$key]
    if ($default -match '^[A-Z][A-Z_0-9]*$') {
        $constant = [regex]::Match($defs, '(?m)^#define\s+' + $default + '\s+(-?\d+)')
        if ($constant.Success) { $default = $constant.Groups[1].Value }
    }
    if ($default -match '^-?\d+$' -and $iniValues["InventoryFilter|$key"] -match '^-?\d+$' -and $default -ne $iniValues["InventoryFilter|$key"]) {
        Fail ("INI key {0}: the default in the code is {1}, the shipped INI has {2}" -f $key, $default, $iniValues["InventoryFilter|$key"])
    }
}
foreach ($kind in 'Melee', 'WLight', 'Weapon', 'Ammo', 'Armor', 'Drug', 'Tool', 'Custom', 'Misc') {
    foreach ($prefix in 'p', 'n') {
        if (-not $iniValues.ContainsKey("HotKeys|$prefix$kind")) { Fail "INI key $prefix$kind (hot key) not in InvenFilter.ini" }
    }
}
if (-not $iniValues.ContainsKey('HotKeys|nPrefix')) { Fail 'INI key nPrefix (hot key) not in InvenFilter.ini' }

# ---- INI comments in every language
$dictionary = [System.IO.File]::ReadAllText((Join-Path $dat 'InvenFilter_comments.json'), $utf8) | ConvertFrom-Json
$temp = Join-Path ([System.IO.Path]::GetTempPath()) ('InvenFilter_check_' + [guid]::NewGuid().ToString('N') + '.ini')
try {
    foreach ($language in @($dictionary.languages)) {
        Copy-Item -LiteralPath $iniPath -Destination $temp -Force
        $output = & (Join-Path $dat 'TranslateIni.ps1') -Language $language -Path $temp -Dictionary (Join-Path $dat 'InvenFilter_comments.json') 3>&1 4>&1 5>&1 6>&1
        foreach ($item in @($output)) {
            if ($item -is [System.Management.Automation.WarningRecord]) { Fail ("comments in {0}: {1}" -f $language, $item.Message) }
        }
    }
} finally {
    Remove-Item -LiteralPath $temp -Force -ErrorAction SilentlyContinue
}
foreach ($block in $dictionary.blocks.PSObject.Properties) {
    foreach ($language in @($dictionary.languages)) {
        $entry = $block.Value.PSObject.Properties[$language]
        if ((-not $entry) -or (@($entry.Value).Count -eq 0)) { Fail "dictionary: block $($block.Name) has no text in $language" }
    }
}

# ---- zips
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem
$dist = Join-Path $Root 'dist'
if (Test-Path -LiteralPath $dist) {
    foreach ($zip in (Get-ChildItem -LiteralPath $dist -Filter 'InventoryFilterSfall*.zip' -File)) {
        $archive = [System.IO.Compression.ZipFile]::OpenRead($zip.FullName)
        try {
            $names = @($archive.Entries | ForEach-Object { $_.FullName })
            if ($names.Count -eq 0) { Fail "$($zip.Name) is empty" }
            if (@($names | Where-Object { $_.Contains('\') }).Count -gt 0) { Fail "$($zip.Name): entry names with a backslash" }
            if ($names -notcontains 'mods/InventoryFilter.dat/scripts/gl_InvenFilter.int') { Fail "$($zip.Name): the compiled script is not in it" }
            if ($names -notcontains 'mods/InventoryFilter.dat/InvenFilter.ini') { Fail "$($zip.Name): InvenFilter.ini is not in it" }
        } finally {
            $archive.Dispose()
        }
    }
}

if ($problems.Count -gt 0) {
    Write-Host ''
    Write-Host "Check: $($problems.Count) problem(s)" -ForegroundColor Red
    foreach ($p in $problems) { Write-Host "  - $p" -ForegroundColor Red }
    throw "Check failed ($($problems.Count) problem(s))"
}
Write-Host ("Check: ok ({0} message ids in {1} files, {2} INI keys, {3} languages)" -f $required.Count, @($msgFiles).Count, $read.Count, @($dictionary.languages).Count) -ForegroundColor Green
