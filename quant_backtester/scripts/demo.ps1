Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Set-Location (Split-Path -Parent $PSScriptRoot)

Write-Host "[demo] Building Debug"
cmake -S . -B build
cmake --build build --config Debug

Write-Host "[demo] Running inline config demo"
& .\scripts\run_experiment.ps1 -BuildType Debug -ConfigPath configs/experiments/default_experiment.json

Write-Host "[demo] Running CSV config demo"
& .\scripts\run_experiment.ps1 -BuildType Debug -ConfigPath configs/experiments/csv_demo_experiment.json

Write-Host "[demo] Completed"
