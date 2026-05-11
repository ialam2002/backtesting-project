#pragma once

#include <memory>
#include <string>
#include <vector>

#include "quant/core/types.h"
#include "quant/strategy/strategy_base.h"

namespace quant {

/**
 * @brief Runtime configuration for a single backtest execution.
 */
struct ExperimentConfig {
    std::string strategy = "moving_average";

    InstrumentId instrument = 1;
    double starting_cash = 100000.0;
    Quantity lot_size = 10;
    double slippage_bps = 2.0;
    double commission_per_share = 0.0;

    std::size_t short_window = 3;
    std::size_t long_window = 5;
    std::size_t lookback = 20;
    double z_threshold = 1.5;
    std::size_t donchian_window = 20;

    std::string prices_csv;
    std::vector<Price> inline_prices;

    std::string artifacts_root = "configs/experiments";
    std::string structured_log_path = "configs/experiments/backtester.log";
};

/**
 * @brief Load experiment config from a JSON file.
 */
ExperimentConfig load_experiment_config(const std::string& file_path);

/**
 * @brief Resolve prices from either CSV input path or inline array from config.
 */
std::vector<Price> resolve_prices(const ExperimentConfig& cfg);

/**
 * @brief Build the configured strategy instance.
 */
std::unique_ptr<StrategyBase> build_strategy(const ExperimentConfig& cfg);

}  // namespace quant
