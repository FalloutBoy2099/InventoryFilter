<#
.SYNOPSIS
    Rewrites the comments of InvenFilter.ini in the language of your choice.

.DESCRIPTION
    The comments come from InvenFilter_comments.json (one translation of every comment block per
    language of the mod). A comment block is found by the key it sits above, e.g. the lines above
    "Enable=1" are the block "Enable"; the run that ends with a commented-out key such as
    ";Language=english" belongs to the block "Language" and that last line is kept as it is.
    Settings, sections and blank lines are never touched. A block that is not in the dictionary is
    left unchanged and reported.

    The file is written as UTF-8 without BOM with its own line endings; sFall and the mod read the
    settings only, so the encoding of the comments does not matter to them.

.PARAMETER Language
    english, french, russian, czech, german, hungarian, italian, polish, portuguese, spanish, cht, korean or swedish.
    When it is not given, a menu asks for it.

.PARAMETER Path
    The INI file to change. Default: InvenFilter.ini next to this script.

.PARAMETER Dictionary
    The JSON file. Default: InvenFilter_comments.json next to this script.

.PARAMETER List
    Only lists the languages of the dictionary.

.EXAMPLE
    .\TranslateIni.ps1 -Language french

.EXAMPLE
    .\TranslateIni.ps1 -Language german -Path C:\Fallout2\mods\InventoryFilter.dat\InvenFilter.ini -WhatIf
#>
[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [string] $Language,
    [string] $Path,
    [string] $Dictionary,
    [switch] $List
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$scriptDir = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
if (-not $Path) { $Path = Join-Path $scriptDir 'InvenFilter.ini' }
if (-not $Dictionary) { $Dictionary = Join-Path $scriptDir 'InvenFilter_comments.json' }

$utf8Strict = New-Object System.Text.UTF8Encoding($false, $true)
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)

if (-not (Test-Path -LiteralPath $Dictionary)) {
    throw "Dictionary not found: $Dictionary"
}
$dict = [System.IO.File]::ReadAllText($Dictionary, $utf8NoBom) | ConvertFrom-Json
$languages = @($dict.languages)

if ($List) {
    $languages
    return
}

# ---- language choice
if (-not $Language) {
    Write-Host 'Languages:'
    for ($n = 0; $n -lt $languages.Count; $n++) {
        Write-Host ('  {0,2}. {1}' -f ($n + 1), $languages[$n])
    }
    $answer = Read-Host 'Number or name of the language'
    if ($answer -match '^\d+$') {
        $index = [int]$answer - 1
        if ($index -lt 0 -or $index -ge $languages.Count) { throw "No language number $answer" }
        $Language = $languages[$index]
    } else {
        $Language = $answer
    }
}
$match = $languages | Where-Object { $_ -ieq $Language } | Select-Object -First 1
if (-not $match) {
    throw ("Unknown language '{0}'. Available: {1}" -f $Language, ($languages -join ', '))
}
$Language = $match

# ---- read the INI (UTF-8, or Windows-1252 if it is not valid UTF-8)
if (-not (Test-Path -LiteralPath $Path)) {
    throw "INI file not found: $Path"
}
$bytes = [System.IO.File]::ReadAllBytes($Path)
try {
    $text = $utf8Strict.GetString($bytes)
} catch {
    $text = [System.Text.Encoding]::GetEncoding(1252).GetString($bytes)
}
if ($text.Length -gt 0 -and $text[0] -eq [char]0xFEFF) { $text = $text.Substring(1) }
$eol = if ($text.Contains("`r`n")) { "`r`n" } else { "`n" }
$lines = [System.Collections.Generic.List[string]]::new()
foreach ($l in ($text -split "\r?\n")) { $lines.Add($l) }
# a final line break gives an empty last element: keep it so that the file ends the same way
$blocks = $dict.blocks

# ---- replace the comment blocks
$output = [System.Collections.Generic.List[string]]::new()
$changed = 0
$unknown = @()
$i = 0
while ($i -lt $lines.Count) {
    if (-not $lines[$i].StartsWith(';')) {
        $output.Add($lines[$i])
        $i++
        continue
    }
    $first = $i
    $last = $i
    while (($last + 1) -lt $lines.Count -and $lines[$last + 1].StartsWith(';')) { $last++ }
    $keep = $null
    $anchor = $null
    if ($lines[$last] -match '^;(\w+)=\S*\s*$') {
        $anchor = $Matches[1]
        $keep = $lines[$last]
        $last--
    } elseif (($last + 1) -lt $lines.Count -and $lines[$last + 1] -match '^(\w+)\s*=') {
        $anchor = $Matches[1]
    }
    $entry = $null
    if ($anchor) {
        $block = $blocks.PSObject.Properties[$anchor]
        if ($block) { $entry = $block.Value.PSObject.Properties[$Language] }
    }
    if ($entry) {
        foreach ($text1 in @($entry.Value)) { $output.Add(';' + $text1) }
        $changed++
    } else {
        for ($k = $first; $k -le $last; $k++) { $output.Add($lines[$k]) }
        $unknown += $(if ($anchor) { $anchor } else { "line $($first + 1)" })
    }
    if ($keep -ne $null) { $output.Add($keep) }
    $i = $(if ($keep -ne $null) { $last + 2 } else { $last + 1 })
}

$result = $output -join $eol
if ($PSCmdlet.ShouldProcess($Path, "Write the comments in $Language")) {
    [System.IO.File]::WriteAllText($Path, $result, $utf8NoBom)
    Write-Host "$changed comment block(s) written in $Language : $Path"
} else {
    Write-Host "$changed comment block(s) would be written in $Language"
}
if ($unknown.Count -gt 0) {
    Write-Warning ("Left unchanged (not in the dictionary): " + ($unknown -join ', '))
}
