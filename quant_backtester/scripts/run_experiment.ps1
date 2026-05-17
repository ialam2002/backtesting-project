param(
    [string]$BuildType = "Debug",
    [string]$ConfigPath = "configs/experiments/default_experiment.json"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Set-Location (Split-Path -Parent $PSScriptRoot)

cmake -S . -B build
cmake --build build --config $BuildType

$exePath = Join-Path -Path ".\build\src\$BuildType" -ChildPath "backtester.exe"
if (-not (Test-Path $exePath)) {
    throw "Built executable not found at: $exePath"
}

& $exePath $ConfigPath
