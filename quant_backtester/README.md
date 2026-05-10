# QuantBacktester

Production-style C++ backtesting skeleton using CMake.

## Structure

- `include/quant`: public module interfaces
- `src`: implementation files and application entry point
- `tests`: unit + integration tests
- `configs`, `data`, `scripts`: runtime and operational assets

## Build

```powershell
cd quant_backtester
cmake -S . -B build
cmake --build build
```

## Run

```powershell
.\build\src\backtester.exe
```

## Test

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

## Next Steps

- Add real CSV ingestion under `data/`.
- Add concrete strategy classes under `strategy/`.
- Extend execution models (slippage, fees, latency).
- Add pybind11 bindings when ready.
