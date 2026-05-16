#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include "quant/core/experiment_config.h"

using namespace quant;

namespace {

std::string write_temp_config(const std::string& file_name, const std::string& json_body) {
    const std::filesystem::path path = std::filesystem::temp_directory_path() / file_name;
    std::ofstream out(path);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to create temp config file");
    }
    out << json_body;
    out.close();
    return path.string();
}

}  // namespace

TEST_CASE("load_experiment_config accepts a valid inline-price config", "[config]") {
    const std::string path = write_temp_config(
        "quant_valid_config.json",
        R"({
            "strategy": "moving_average",
            "instrument": 1,
            "starting_cash": 100000.0,
            "lot_size": 10,
            "slippage_bps": 2.0,
            "commission_per_share": 0.01,
            "short_window": 3,
            "long_window": 5,
            "lookback": 20,
            "z_threshold": 1.5,
            "donchian_window": 20,
            "prices_csv": "",
            "inline_prices": [100.0, 101.0, 102.0],
            "artifacts_root": "configs/experiments",
            "structured_log_path": "configs/experiments/backtester.log"
        })");

    const auto cfg = load_experiment_config(path);

    REQUIRE(cfg.strategy == "moving_average");
    REQUIRE(cfg.inline_prices.size() == 3);
    REQUIRE(cfg.prices_csv.empty());

    std::filesystem::remove(path);
}

TEST_CASE("load_experiment_config rejects negative commission", "[config]") {
    const std::string path = write_temp_config(
        "quant_invalid_commission_config.json",
        R"({
            "strategy": "moving_average",
            "instrument": 1,
            "starting_cash": 100000.0,
            "lot_size": 10,
            "slippage_bps": 2.0,
            "commission_per_share": -0.01,
            "short_window": 3,
            "long_window": 5,
            "lookback": 20,
            "z_threshold": 1.5,
            "donchian_window": 20,
            "prices_csv": "",
            "inline_prices": [100.0, 101.0, 102.0],
            "artifacts_root": "configs/experiments",
            "structured_log_path": "configs/experiments/backtester.log"
        })");

    REQUIRE_THROWS_WITH(load_experiment_config(path), "commission_per_share must be >= 0");

    std::filesystem::remove(path);
}

TEST_CASE("load_experiment_config rejects ambiguous price sources", "[config]") {
    const std::string path = write_temp_config(
        "quant_ambiguous_prices_config.json",
        R"({
            "strategy": "moving_average",
            "instrument": 1,
            "starting_cash": 100000.0,
            "lot_size": 10,
            "slippage_bps": 2.0,
            "commission_per_share": 0.01,
            "short_window": 3,
            "long_window": 5,
            "lookback": 20,
            "z_threshold": 1.5,
            "donchian_window": 20,
            "prices_csv": "data/sample.csv",
            "inline_prices": [100.0, 101.0, 102.0],
            "artifacts_root": "configs/experiments",
            "structured_log_path": "configs/experiments/backtester.log"
        })");

    REQUIRE_THROWS_WITH(
        load_experiment_config(path),
        "Configure exactly one price source: prices_csv or inline_prices");

    std::filesystem::remove(path);
}

TEST_CASE("load_experiment_config rejects invalid moving average windows", "[config]") {
    const std::string path = write_temp_config(
        "quant_invalid_ma_windows_config.json",
        R"({
            "strategy": "moving_average",
            "instrument": 1,
            "starting_cash": 100000.0,
            "lot_size": 10,
            "slippage_bps": 2.0,
            "commission_per_share": 0.01,
            "short_window": 5,
            "long_window": 5,
            "lookback": 20,
            "z_threshold": 1.5,
            "donchian_window": 20,
            "prices_csv": "",
            "inline_prices": [100.0, 101.0, 102.0],
            "artifacts_root": "configs/experiments",
            "structured_log_path": "configs/experiments/backtester.log"
        })");

    REQUIRE_THROWS_WITH(load_experiment_config(path), "short_window must be < long_window");

    std::filesystem::remove(path);
}
