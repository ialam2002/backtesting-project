# QuantBacktester

Production-style C++ backtesting engine using CMake.

## Structure

- `include/quant`: public module interfaces
- `src`: implementation files and application entry point
- `tests`: unit + integration tests
- `configs`, `data`, `scripts`: runtime and operational assets

## Architecture Notes

- Event-driven pipeline: Market -> Signal -> Order -> Fill -> Portfolio -> Metrics.
- OMS is split into `SignalHandler`, `OrderGenerator`, and `OrderRouter`.
- Risk checks are enforced by `RiskManager` before orders are accepted.
- Deterministic replay is supported through `EventLogger` + `ReplayEngine` CSV logs.
- Data loading supports one-column price CSV, OHLCV CSV normalization, and optional Parquet.
- Execution supports partial fill simulation via `FillSimulator`.
- Analytics includes Sharpe, Sortino, volatility, win/loss ratio, and max drawdown.
- Each run writes artifacts under `configs/experiments/<run_id>/`.
- Runtime is config-driven via JSON experiment files (see `configs/experiments/default_experiment.json`).
- Structured logging writes JSON lines to `configs/experiments/backtester.log`.
- CI is configured in `.github/workflows/ci.yml` for Windows + Linux Debug/Release build/test.

## Build

```powershell
cd quant_backtester
cmake -S . -B build
cmake --build build --config Debug
cmake --build build --config Release
```

## Run

```powershell
.\build\src\Debug\backtester.exe configs/experiments/default_experiment.json
```

Or use helper scripts:

```powershell
.\scripts\run_experiment.ps1 -BuildType Debug -ConfigPath configs/experiments/default_experiment.json
```

```bash
./scripts/run_experiment.sh Debug configs/experiments/default_experiment.json
```

CSV demo:

```powershell
.\scripts\run_experiment.ps1 -BuildType Debug -ConfigPath configs/experiments/csv_demo_experiment.json
```

## Experiment Config

`default_experiment.json` supports:

- strategy selection: `moving_average`, `mean_reversion`, `donchian_breakout`
- execution params: `lot_size`, `slippage_bps`, `commission_per_share`
- risk limits: `risk_max_order_qty`, `risk_max_abs_position_per_instrument`, `risk_max_gross_notional`
- data source: `prices_csv` or `inline_prices`
- runtime outputs: `artifacts_root`, `structured_log_path`

Validation rules enforce positive cash/lot/limits, non-negative costs, strategy-specific parameter constraints, and exactly one price source.

## Data Format

- `data/sample_prices.csv`: single-column `price` CSV for quick demos.
- `load_prices_from_csv` fails on malformed rows after an optional header line.
- `load_ohlcv_from_csv` expects columns: `timestamp,open,high,low,close,volume`.

## Test

```powershell
ctest --test-dir build -C Debug --output-on-failure
ctest --test-dir build -C Release --output-on-failure
```

## Install and Package

```powershell
cmake --install build --config Release --prefix install
```

Create package archive:

```powershell
cmake --build build --config Release --target package
```

## Demo Checklist

- Run all tests in Debug and Release.
- Run inline demo config.
- Run CSV demo config using `data/sample_prices.csv`.
- Verify report output and generated run artifacts under `configs/experiments/`.
