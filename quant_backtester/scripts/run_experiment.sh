#!/usr/bin/env bash
set -euo pipefail

BUILD_TYPE="${1:-Debug}"
CONFIG_PATH="${2:-configs/experiments/default_experiment.json}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

cd "${REPO_ROOT}"

cmake -S . -B build -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"
cmake --build build --config "${BUILD_TYPE}"

if [[ "${BUILD_TYPE}" == "Debug" || "${BUILD_TYPE}" == "Release" || "${BUILD_TYPE}" == "RelWithDebInfo" || "${BUILD_TYPE}" == "MinSizeRel" ]]; then
    if [[ -x "./build/src/backtester" ]]; then
        ./build/src/backtester "${CONFIG_PATH}"
    elif [[ -x "./build/src/${BUILD_TYPE}/backtester" ]]; then
        "./build/src/${BUILD_TYPE}/backtester" "${CONFIG_PATH}"
    elif [[ -x "./build/src/${BUILD_TYPE}/backtester.exe" ]]; then
        "./build/src/${BUILD_TYPE}/backtester.exe" "${CONFIG_PATH}"
    else
        echo "Unable to locate built backtester binary" >&2
        exit 1
    fi
else
    echo "Unsupported build type: ${BUILD_TYPE}" >&2
    exit 1
fi
