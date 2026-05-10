param(
    [string]$BuildType = "Debug"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

Set-Location (Split-Path -Parent $PSScriptRoot)

cmake -S . -B build
cmake --build build --config $BuildType

if ($BuildType -eq "Debug") {
    .\build\src\Debug\backtester.exe
} else {
    .\build\src\Release\backtester.exe
}
