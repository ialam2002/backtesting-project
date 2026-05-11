#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>

#include "quant/analytics/performance_metrics.h"

using namespace quant;

TEST_CASE("Performance metrics produce finite non-zero values", "[analytics]") {
    const std::vector<double> returns = {0.01, -0.005, 0.02, -0.01, 0.015};

    REQUIRE(volatility(returns) > 0.0);
    REQUIRE(sharpe_ratio(returns) != Approx(0.0));
    REQUIRE(sortino_ratio(returns) != Approx(0.0));
    REQUIRE(win_loss_ratio(returns) > 0.0);
}

TEST_CASE("max_drawdown returns zero for monotonically rising equity", "[analytics]") {
    const std::vector<double> equity = {100.0, 101.0, 102.0, 103.0};
    REQUIRE(max_drawdown(equity) == Approx(0.0));
}

TEST_CASE("max_drawdown detects correct peak-to-trough", "[analytics]") {
    // Drops from 100 to 50: drawdown = 0.5
    const std::vector<double> equity = {100.0, 90.0, 80.0, 50.0, 60.0};
    REQUIRE(max_drawdown(equity) == Approx(0.5));
}
