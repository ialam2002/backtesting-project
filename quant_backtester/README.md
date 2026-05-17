# QuantBacktester

QuantBacktester is a C++17 event-driven backtesting engine for reproducible strategy research and execution-aware simulation.

## Design Goals

- Deterministic, replayable runs with persisted artifacts
- Clear separation between alpha generation, OMS, execution simulation, and portfolio accounting
- Config-driven experiments with strict runtime validation
- Realistic execution assumptions: slippage, transaction costs, and partial fills
- Explicit pre-trade risk gating before order acceptance

## System Architecture

Pipeline:

Market -> Strategy -> Signal -> OMS -> Execution -> Portfolio -> Analytics

Core modules:

- `src/core`: simulation engine, experiment config parsing/validation, walk-forward optimizer
- `src/strategy`: moving average crossover, mean reversion, Donchian breakout
- `src/execution`: signal handling, order generation/routing, slippage and fill simulation
- `src/portfolio`: portfolio state, position tracking, risk manager, PnL decomposition
- `src/data`: CSV loaders, OHLCV normalization, optional Parquet path
- `src/analytics`: Sharpe, Sortino, drawdown, CAGR, Calmar and report generation

## Verification Snapshot

- Test suite: 18/18 passing in Debug and Release
- Verified run outputs from current workspace:

| Scenario | Final Equity | Sharpe | Max Drawdown |
| --- | ---: | ---: | ---: |
| Inline config | 99965.5 | -4.69859 | 0.00041267 |
| CSV config | 100004 | 0.631345 | 0.000235075 |

Each run emits a unique artifact directory under `configs/experiments/<run_id>/` containing `report.txt` and `events.csv`.

## Quick Start

```powershell
cd quant_backtester
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
ctest --test-dir build -C Release --output-on-failure
```

Run inline-data backtest:

```powershell
.\build\src\Debug\backtester.exe configs/experiments/default_experiment.json
```

Run CSV-data backtest:

```powershell
.\build\src\Debug\backtester.exe configs/experiments/csv_demo_experiment.json
```

## Experiment Configuration

Key fields:

- Strategy and strategy-specific parameters
- Execution inputs: `lot_size`, `slippage_bps`, `commission_per_share`
- Risk limits: `risk_max_order_qty`, `risk_max_abs_position_per_instrument`, `risk_max_gross_notional`
- Exactly one active price source: `prices_csv` or `inline_prices`
- Artifact and structured log output paths

Validation enforces positive quantities/limits, non-negative costs, valid strategy parameter relationships, and a single active price source.

Reference configs:

- `configs/experiments/default_experiment.json`
- `configs/experiments/csv_demo_experiment.json`

## Data

- Sample CSV: `data/sample_prices.csv` (single `price` column)

## Tech Stack

- Language: C++17
- Build system: CMake
- Tests: Catch2
- CI: GitHub Actions (Windows/Linux, Debug/Release)
