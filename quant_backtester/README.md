# QuantBacktester

Production-style C++ backtesting skeleton using CMake.

## Structure

- `include/quant`: public module interfaces
- `src`: implementation files and application entry point
- `tests`: unit + integration tests
- `configs`, `data`, `scripts`: runtime and operational assets

## Architecture Notes

- Event-driven pipeline: Market -> Signal -> Order -> Fill -> Portfolio -> Metrics.
- OMS is split into `SignalHandler`, `OrderGenerator`, and `OrderRouter`.
- Deterministic replay is supported through `EventLogger` + `ReplayEngine` CSV logs.
- Data normalization supports OHLCV schema validation and timestamp normalization.
- Execution supports partial fill simulation via `FillSimulator`.
- Analytics includes Sharpe, Sortino, volatility, win/loss ratio, and max drawdown.
- Each run writes artifacts under `configs/experiments/<run_id>/`.
- Runtime is config-driven via JSON experiment files (see `configs/experiments/default_experiment.json`).
- Structured logging writes JSON lines to `configs/experiments/backtester.log`.
- CI is configured in `.github/workflows/ci.yml` for Windows + Linux build/test.

## Build

```powershell
cd quant_backtester
cmake -S . -B build
cmake --build build
```

## Run

```powershell
.\build\src\backtester.exe configs/experiments/default_experiment.json
```

Or use the helper script:

```powershell
.\scripts\run_experiment.ps1 -BuildType Debug -ConfigPath configs/experiments/default_experiment.json
```

## Experiment Config

`default_experiment.json` supports:

- strategy selection: `moving_average`, `mean_reversion`, `donchian_breakout`
- execution params: `lot_size`, `slippage_bps`, `commission_per_share`
- data source: `prices_csv` or `inline_prices`
- runtime outputs: `artifacts_root`, `structured_log_path`

## Test

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

## Next Steps

- Add real CSV ingestion under `data/`.
- Add concrete strategy classes under `strategy/`.
- Extend execution models (slippage, fees, latency).
- Add pybind11 bindings when ready.
