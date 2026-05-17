#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

cd "${REPO_ROOT}"

echo "[demo] Building Debug"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug

echo "[demo] Running inline config demo"
./scripts/run_experiment.sh Debug configs/experiments/default_experiment.json

echo "[demo] Running CSV config demo"
./scripts/run_experiment.sh Debug configs/experiments/csv_demo_experiment.json

echo "[demo] Completed"
