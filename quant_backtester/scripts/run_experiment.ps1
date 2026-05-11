param(
    [string]$BuildType = "Debug",
    [string]$ConfigPath = "configs/experiments/default_experiment.json"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Set-Location (Split-Path -Parent $PSScriptRoot)

cmake -S . -B build
cmake --build build --config $BuildType

if ($BuildType -eq "Debug") {
    .\build\src\Debug\backtester.exe $ConfigPath
} else {
    .\build\src\Release\backtester.exe $ConfigPath
}
