param(
    [ValidateSet('all', 'sfall5', 'sfall4')]
    [string] $Target = 'all',
    [switch] $Clean,
    [switch] $NoPackage,
    [switch] $Trace
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$Root = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$Root = [System.IO.Path]::GetFullPath($Root)
$BuildRoot = Join-Path $Root 'build'
$DistRoot = Join-Path $Root 'dist'
$SourceRoot = Join-Path $Root 'source'
$DatRoot = Join-Path $Root 'InventoryFilter.dat'

# 'all' (the default) builds both targets, one after the other: dist\InventoryFilterSfall5.zip and
# dist\InventoryFilterSfall4.zip. -Clean is done once, before the first.
if ($Target -eq 'all') {
    if ($Clean) {
        foreach ($folder in @($BuildRoot, $DistRoot)) {
            if (Test-Path -LiteralPath $folder) { Remove-Item -LiteralPath $folder -Recurse -Force }
        }
    }
    foreach ($one in @('sfall5', 'sfall4')) {
        & $PSCommandPath -Target $one -NoPackage:$NoPackage -Trace:$Trace
        if (-not $?) { throw "Build failed for $one" }
    }
    return
}

# sfall5 builds in build\, sfall4 in build\sfall4.
if ($Target -eq 'sfall4') {
    $Compiler = Join-Path $Root 'tools\sslc-sfall4\compile.exe'
    $TargetBuild = Join-Path $BuildRoot 'sfall4'
    $ZipName = 'InventoryFilterSfall4.zip'
    $CompilerExtraArgs = @('-mSFALL4')
} else {
    $Compiler = Join-Path $Root 'tools\sslc-sfall5\compile.exe'
    $TargetBuild = $BuildRoot
    $ZipName = 'InventoryFilterSfall5.zip'
    $CompilerExtraArgs = @()
}
# -Trace builds a variant that logs what the script does to mods\InventoryFilter.dat\InvenTrace.ini.
# The compiler keeps a single -m macro: SFALL4_TRACE stands for SFALL4 + INVDEBUG (see gl_InvenFilter.ssl).
if ($Trace) { $CompilerExtraArgs = if ($Target -eq 'sfall4') { @('-mSFALL4_TRACE') } else { @('-mINVDEBUG') } }
# A trace build is never packaged under the name of the release.
if ($Trace) { $ZipName = $ZipName -replace '\.zip$', '_trace.zip' }
$CompiledScripts = Join-Path $TargetBuild 'compiled\scripts'

function Write-Step {
    param([Parameter(Mandatory)][string] $Message)
    Write-Host ''
    Write-Host "==> $Message" -ForegroundColor Cyan
}

function Resolve-UnderRoot {
    param([Parameter(Mandatory)][string] $Path)
    $full = [System.IO.Path]::GetFullPath($Path)
    $rootWithSlash = $Root.TrimEnd('\') + '\'
    if (($full -ne $Root) -and (-not $full.StartsWith($rootWithSlash, [System.StringComparison]::OrdinalIgnoreCase))) {
        throw "Refusing to operate outside root '$Root': $full"
    }
    return $full
}

function Reset-Directory {
    param([Parameter(Mandatory)][string] $Path)
    $full = Resolve-UnderRoot $Path
    if (Test-Path -LiteralPath $full) {
        Remove-Item -LiteralPath $full -Recurse -Force
    }
    New-Item -ItemType Directory -Path $full -Force | Out-Null
}

function Ensure-Directory {
    param([Parameter(Mandatory)][string] $Path)
    $full = Resolve-UnderRoot $Path
    New-Item -ItemType Directory -Path $full -Force | Out-Null
}

function Copy-DirectoryMirror {
    param(
        [Parameter(Mandatory)][string] $Source,
        [Parameter(Mandatory)][string] $Destination
    )
    if (-not (Test-Path -LiteralPath $Source)) {
        throw "Source directory not found: $Source"
    }
    Ensure-Directory $Destination
    & robocopy $Source $Destination /MIR /NFL /NDL /NJH /NJS /NP | Out-Null
    $code = $LASTEXITCODE
    if ($code -gt 7) {
        throw "robocopy failed with exit code ${code}: $Source -> $Destination"
    }
    $global:LASTEXITCODE = 0
}

function Invoke-Checked {
    param(
        [Parameter(Mandatory)][string] $FilePath,
        [Parameter(Mandatory)][string[]] $Arguments,
        [Parameter(Mandatory)][string] $WorkingDirectory
    )

    Push-Location $WorkingDirectory
    try {
        $output = & $FilePath @Arguments
        $code = $LASTEXITCODE
        if ($output) {
            $output | ForEach-Object { Write-Host $_ }
        }
        if ($code -ne 0) {
            throw "Command failed with exit code ${code}: $FilePath $($Arguments -join ' ')"
        }
    } finally {
        Pop-Location
    }
}

if ($Clean) {
    Write-Step 'Cleaning build output'
    Reset-Directory $BuildRoot
    Reset-Directory $DistRoot
} else {
    Ensure-Directory $BuildRoot
    Ensure-Directory $DistRoot
}

if (-not (Test-Path -LiteralPath $Compiler)) {
    throw "Compiler not found: $Compiler"
}
if (-not (Test-Path -LiteralPath (Join-Path $SourceRoot 'gl_InvenFilter.ssl'))) {
    throw "Source file not found: $SourceRoot\gl_InvenFilter.ssl"
}
if (-not (Test-Path -LiteralPath $DatRoot)) {
    throw "Mod data directory not found: $DatRoot"
}

Write-Step "Compiling InventoryFilter ($Target)"
Reset-Directory $CompiledScripts
$outFile = Join-Path $CompiledScripts 'gl_InvenFilter.int'
Invoke-Checked -FilePath $Compiler `
    -Arguments (@('-l', '-O2', '-p', '-s', '-q') + $CompilerExtraArgs + @('gl_InvenFilter.ssl', '-o', $outFile)) `
    -WorkingDirectory $SourceRoot

if (-not (Test-Path -LiteralPath $outFile)) {
    throw "Compiler did not create expected file: $outFile"
}

if ($NoPackage) {
    Write-Step 'Done'
    return
}

Write-Step "Packaging InventoryFilter ($Target)"
$stage = Join-Path $TargetBuild 'package'
$stageDat = Join-Path $stage 'mods\InventoryFilter.dat'
Reset-Directory $stage
Copy-DirectoryMirror -Source $DatRoot -Destination $stageDat
Copy-DirectoryMirror -Source $CompiledScripts -Destination (Join-Path $stageDat 'scripts')

$zipFile = Join-Path $DistRoot $ZipName
Remove-Item -LiteralPath $zipFile -Force -ErrorAction SilentlyContinue
# The entries are written one by one: CreateFromDirectory of Windows PowerShell 5.1 names them with
# backslashes, which mod managers and non-Windows extractors do not understand.
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem
$archive = [System.IO.Compression.ZipFile]::Open($zipFile, [System.IO.Compression.ZipArchiveMode]::Create)
try {
    foreach ($file in Get-ChildItem -LiteralPath $stage -Recurse -File) {
        $entryName = $file.FullName.Substring($stage.Length).TrimStart([char]92, [char]47).Replace([string][char]92, '/')
        [void][System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile($archive, $file.FullName, $entryName, [System.IO.Compression.CompressionLevel]::Optimal)
    }
}
finally {
    $archive.Dispose()
}
Write-Host "Created $zipFile" -ForegroundColor Green

Write-Step 'Done'
