param(
    [Parameter(Mandatory=$true)]
    [string]$Version
)

$ErrorActionPreference = "Stop"

$Root      = "C:\MR_NagramXF_Yukigram\tdesktop"
$BuildDir  = Join-Path $Root "out"
$Exe       = Join-Path $BuildDir "SushiGram.exe"
$Iss       = Join-Path $Root "SushiGram-Setup.iss"
$Installer = Join-Path $Root "Installer\SushiGram-Windows-x64-Setup.exe"
$Inno      = "C:\Users\Mahdi\AppData\Local\Programs\Inno Setup 6\ISCC.exe"

$ReleaseRoot = Join-Path $Root "Releases"
$ReleaseDir  = Join-Path $ReleaseRoot "SushiGram-v$Version"
$PortableDir = Join-Path $ReleaseDir "Portable"

Write-Host ""
Write-Host "============================================================"
Write-Host "       SushiGram Release Builder v$Version"
Write-Host "============================================================"
Write-Host ""

# ------------------------------------------------------------
# 1. Check files
# ------------------------------------------------------------

if (!(Test-Path $Iss)) {
    throw "SushiGram-Setup.iss not found."
}

if (!(Test-Path $Inno)) {
    throw "Inno Setup compiler not found: $Inno"
}

# ------------------------------------------------------------
# 2. Build SushiGram
# ------------------------------------------------------------

# Verify that the Ninja build directory is a real Release build.
$CMakeCache = Join-Path $BuildDir "CMakeCache.txt"

if (!(Test-Path $CMakeCache)) {
    throw "CMakeCache.txt not found. Configure the project as Release first."
}

$BuildTypeLine = Get-Content $CMakeCache |
    Where-Object { $_ -match '^CMAKE_BUILD_TYPE:' } |
    Select-Object -First 1

if ($BuildTypeLine -notmatch '=Release$') {
    Write-Host ""
    Write-Host "ERROR: OUT DIRECTORY IS NOT A RELEASE BUILD!" -ForegroundColor Red
    Write-Host "Current: $BuildTypeLine" -ForegroundColor Yellow
    throw "Refusing to package a non-Release build."
}

Write-Host "      CMAKE BUILD TYPE: RELEASE OK" -ForegroundColor Green
Write-Host "[1/6] Building SushiGram..." -ForegroundColor Cyan

Set-Location $Root

cmake --build out --target Telegram --config Release

if ($LASTEXITCODE -ne 0) {
    throw "SushiGram build FAILED."
}

if (!(Test-Path $Exe)) {
    throw "Built SushiGram EXE not found."
}

Write-Host "      BUILD OK" -ForegroundColor Green

# ------------------------------------------------------------
# 3. Prepare release folder
# ------------------------------------------------------------

Write-Host "[2/6] Preparing release folder..." -ForegroundColor Cyan

if (Test-Path $ReleaseDir) {
    Remove-Item $ReleaseDir -Recurse -Force
}

New-Item -ItemType Directory -Path $PortableDir -Force | Out-Null

# ------------------------------------------------------------
# 4. Create Portable
# ------------------------------------------------------------

Write-Host "[3/6] Creating Portable..." -ForegroundColor Cyan

$PortableExe = Join-Path $PortableDir "SushiGram.exe"

Copy-Item $Exe $PortableExe -Force

$PortableZip = Join-Path $ReleaseDir "SushiGram-v$Version-Windows-x64-Portable.zip"

Compress-Archive `
    -Path "$PortableDir\*" `
    -DestinationPath $PortableZip `
    -CompressionLevel Optimal `
    -Force

Write-Host "      PORTABLE OK" -ForegroundColor Green

# ------------------------------------------------------------
# 5. Update and build Installer
# ------------------------------------------------------------

Write-Host "[4/6] Building Installer..." -ForegroundColor Cyan

$IssText = Get-Content $Iss -Raw

$IssText = $IssText -replace `
    '#define MyAppVersion "[^"]+"', `
    "#define MyAppVersion `"$Version`""

Set-Content $Iss $IssText -Encoding UTF8

& $Inno $Iss

if ($LASTEXITCODE -ne 0) {
    throw "Installer build FAILED."
}

if (!(Test-Path $Installer)) {
    throw "Installer EXE not found."
}

$FinalInstaller = Join-Path $ReleaseDir "SushiGram-v$Version-Windows-x64-Setup.exe"

Copy-Item $Installer $FinalInstaller -Force

Write-Host "      INSTALLER OK" -ForegroundColor Green

# ------------------------------------------------------------
# 6. SHA256
# ------------------------------------------------------------

Write-Host "[5/6] Calculating SHA256..." -ForegroundColor Cyan

$ExeHash       = (Get-FileHash $PortableExe -Algorithm SHA256).Hash
$PortableHash  = (Get-FileHash $PortableZip -Algorithm SHA256).Hash
$InstallerHash = (Get-FileHash $FinalInstaller -Algorithm SHA256).Hash

$Commit = (git rev-parse --short=10 HEAD).Trim()
$Tag    = (git tag --points-at HEAD | Select-Object -First 1)

$HashFile = Join-Path $ReleaseDir "SHA256.txt"

@"
SushiGram v$Version
============================================================

Git Commit:
$Commit

Git Tag:
$Tag

SushiGram.exe
SHA256:
$ExeHash

SushiGram-v$Version-Windows-x64-Portable.zip
SHA256:
$PortableHash

SushiGram-v$Version-Windows-x64-Setup.exe
SHA256:
$InstallerHash
"@ | Set-Content $HashFile -Encoding UTF8

Write-Host "      HASHES OK" -ForegroundColor Green

# ------------------------------------------------------------
# Finished
# ------------------------------------------------------------

Write-Host ""
Write-Host "[6/6] RELEASE COMPLETE" -ForegroundColor Green
Write-Host ""
Write-Host "Version:   $Version"
Write-Host "Commit:    $Commit"
Write-Host "Tag:       $Tag"
Write-Host ""
Write-Host "EXE SHA256:"
Write-Host $ExeHash
Write-Host ""
Write-Host "Portable SHA256:"
Write-Host $PortableHash
Write-Host ""
Write-Host "Installer SHA256:"
Write-Host $InstallerHash
Write-Host ""
Write-Host "Output:"
Write-Host $ReleaseDir
Write-Host ""

explorer.exe $ReleaseDir
