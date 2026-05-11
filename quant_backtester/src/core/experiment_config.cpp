#include "quant/core/experiment_config.h"

#include <fstream>
#include <stdexcept>

#include "nlohmann/json.hpp"

#include "quant/data/data_loader.h"
#include "quant/strategy/donchian_breakout.h"
#include "quant/strategy/mean_reversion.h"
#include "quant/strategy/moving_average.h"

namespace quant {

ExperimentConfig load_experiment_config(const std::string& file_path) {
    // Parse a single JSON config into a strongly typed runtime config object.
    std::ifstream in(file_path);
    if (!in.is_open()) {
        throw std::runtime_error("Unable to open config file: " + file_path);
    }

    nlohmann::json j;
    in >> j;

    ExperimentConfig cfg;

    cfg.strategy = j.value("strategy", cfg.strategy);
    cfg.instrument = j.value("instrument", cfg.instrument);
    cfg.starting_cash = j.value("starting_cash", cfg.starting_cash);
    cfg.lot_size = j.value("lot_size", cfg.lot_size);
    cfg.slippage_bps = j.value("slippage_bps", cfg.slippage_bps);
    cfg.commission_per_share = j.value("commission_per_share", cfg.commission_per_share);

    cfg.short_window = j.value("short_window", cfg.short_window);
    cfg.long_window = j.value("long_window", cfg.long_window);
    cfg.lookback = j.value("lookback", cfg.lookback);
    cfg.z_threshold = j.value("z_threshold", cfg.z_threshold);
    cfg.donchian_window = j.value("donchian_window", cfg.donchian_window);

    // Price source can be either an external CSV path or inline numeric data.
    cfg.prices_csv = j.value("prices_csv", cfg.prices_csv);
    if (j.contains("inline_prices") && j["inline_prices"].is_array()) {
        cfg.inline_prices = j["inline_prices"].get<std::vector<Price>>();
    }

    cfg.artifacts_root = j.value("artifacts_root", cfg.artifacts_root);
    cfg.structured_log_path = j.value("structured_log_path", cfg.structured_log_path);

    return cfg;
}

std::vector<Price> resolve_prices(const ExperimentConfig& cfg) {
    // Prefer file-based prices when provided; otherwise use inline fallback.
    if (!cfg.prices_csv.empty()) {
        return load_prices_from_csv(cfg.prices_csv);
    }
    if (!cfg.inline_prices.empty()) {
        return cfg.inline_prices;
    }
    throw std::runtime_error("No price source configured. Set prices_csv or inline_prices.");
}

std::unique_ptr<StrategyBase> build_strategy(const ExperimentConfig& cfg) {
    // Strategy names are intentionally explicit to keep config-driven runs predictable.
    if (cfg.strategy == "moving_average") {
        return std::make_unique<MovingAverageCrossStrategy>(cfg.short_window, cfg.long_window);
    }
    if (cfg.strategy == "mean_reversion") {
        return std::make_unique<MeanReversionStrategy>(cfg.lookback, cfg.z_threshold);
    }
    if (cfg.strategy == "donchian_breakout") {
        return std::make_unique<DonchianBreakoutStrategy>(cfg.donchian_window);
    }

    throw std::runtime_error("Unsupported strategy in config: " + cfg.strategy);
}

}  // namespace quant
