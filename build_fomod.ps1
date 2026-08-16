$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$VersionFile = Join-Path $Root "VERSION"
$Package = Join-Path $Root "package"
$ProviderSource = Join-Path $Root "providers"
$FomodSource = Join-Path $Root "fomod"
$Build = Join-Path $Root "build"
$Stage = Join-Path $Build "fomod-stage"
$Dist = Join-Path $Root "dist"

if (-not (Test-Path -LiteralPath $VersionFile)) {
    throw "VERSION file not found: $VersionFile"
}
$Version = (Get-Content -LiteralPath $VersionFile -Raw).Trim()
if (-not $Version) {
    throw "VERSION file is empty."
}

$RequiredCore = @(
    (Join-Path $Package "SKSE\Plugins\BodyHairSliders.dll"),
    (Join-Path $Package "SKSE\Plugins\BodyHairSliders\config.json"),
    (Join-Path $Package "Scripts\BodyHairSliders.pex"),
    (Join-Path $Package "Scripts\ak_all_in_one_script.pex")
)
foreach ($Path in $RequiredCore) {
    if (-not (Test-Path -LiteralPath $Path)) {
        throw "Required core file missing: $Path"
    }
}

$ProviderPackages = @(
    @{ Stage = "10 Nordic Warmaiden"; File = "nordic_warmaiden.json" },
    @{ Stage = "20 HIMBO"; File = "himbo.json" },
    @{ Stage = "30 Pubes Forever Female"; File = "pubes_forever_female.json" },
    @{ Stage = "40 Pubes Forever Male"; File = "pubes_forever_male.json" },
    @{ Stage = "50 OPubes"; File = "opubes.json" }
)

$ModuleConfig = Join-Path $FomodSource "ModuleConfig.xml"
$Info = Join-Path $FomodSource "info.xml"
foreach ($Path in @($ModuleConfig, $Info)) {
    if (-not (Test-Path -LiteralPath $Path)) {
        throw "Required FOMOD metadata missing: $Path"
    }
}

foreach ($Provider in $ProviderPackages) {
    $Path = Join-Path $ProviderSource $Provider.File
    if (-not (Test-Path -LiteralPath $Path)) {
        throw "Provider config missing: $Path"
    }
}

if (Test-Path -LiteralPath $Stage) {
    Remove-Item -LiteralPath $Stage -Recurse -Force
}
New-Item -ItemType Directory -Force -Path $Stage | Out-Null

$CoreStage = Join-Path $Stage "00 Core"
New-Item -ItemType Directory -Force -Path $CoreStage | Out-Null
Copy-Item (Join-Path $Package "*") $CoreStage -Recurse -Force

foreach ($Provider in $ProviderPackages) {
    $ProviderStage = Join-Path $Stage $Provider.Stage
    $Destination = Join-Path $ProviderStage "SKSE\Plugins\BodyHairSliders\providers"
    New-Item -ItemType Directory -Force -Path $Destination | Out-Null
    Copy-Item (Join-Path $ProviderSource $Provider.File) (Join-Path $Destination $Provider.File) -Force
}

$FomodStage = Join-Path $Stage "fomod"
New-Item -ItemType Directory -Force -Path $FomodStage | Out-Null
Copy-Item (Join-Path $FomodSource "*") $FomodStage -Recurse -Force

$StagedModuleConfig = Join-Path $FomodStage "ModuleConfig.xml"
$StagedInfo = Join-Path $FomodStage "info.xml"
foreach ($Path in @($StagedModuleConfig, $StagedInfo)) {
    $Content = Get-Content -LiteralPath $Path -Raw
    $Content = $Content.Replace("@VERSION@", $Version)
    Set-Content -LiteralPath $Path -Value $Content -Encoding UTF8
}

try {
    [void][xml](Get-Content -LiteralPath $StagedModuleConfig -Raw)
    [void][xml](Get-Content -LiteralPath $StagedInfo -Raw)
} catch {
    throw "Invalid staged FOMOD XML: $($_.Exception.Message)"
}

New-Item -ItemType Directory -Force -Path $Dist | Out-Null
$Zip = Join-Path $Dist "BodyHairSliders-v$Version-FOMOD.zip"
if (Test-Path -LiteralPath $Zip) {
    Remove-Item -LiteralPath $Zip -Force
}

Compress-Archive -Path (Join-Path $Stage "*") -DestinationPath $Zip -CompressionLevel Optimal -Force

Add-Type -AssemblyName System.IO.Compression.FileSystem
$Archive = [System.IO.Compression.ZipFile]::OpenRead($Zip)
try {
    $Entries = @($Archive.Entries | ForEach-Object { $_.FullName.Replace('\', '/') })
    $RequiredEntries = @(
        "00 Core/SKSE/Plugins/BodyHairSliders.dll",
        "00 Core/SKSE/Plugins/BodyHairSliders/config.json",
        "00 Core/Scripts/BodyHairSliders.pex",
        "00 Core/Scripts/ak_all_in_one_script.pex",
        "10 Nordic Warmaiden/SKSE/Plugins/BodyHairSliders/providers/nordic_warmaiden.json",
        "20 HIMBO/SKSE/Plugins/BodyHairSliders/providers/himbo.json",
        "30 Pubes Forever Female/SKSE/Plugins/BodyHairSliders/providers/pubes_forever_female.json",
        "40 Pubes Forever Male/SKSE/Plugins/BodyHairSliders/providers/pubes_forever_male.json",
        "50 OPubes/SKSE/Plugins/BodyHairSliders/providers/opubes.json",
        "fomod/ModuleConfig.xml",
        "fomod/info.xml"
    )
    foreach ($Entry in $RequiredEntries) {
        if ($Entries -notcontains $Entry) {
            throw "Required FOMOD archive entry missing: $Entry"
        }
    }
} finally {
    $Archive.Dispose()
}

Write-Host ""
Write-Host "OK - verified FOMOD package created:" -ForegroundColor Green
Write-Host $Zip
