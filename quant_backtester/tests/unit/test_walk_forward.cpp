#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <memory>
#include <vector>

#include "quant/core/walk_forward.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/strategy/moving_average.h"
#include "quant/strategy/donchian_breakout.h"

using namespace quant;

// 60-bar synthetic trending price series
static std::vector<Price> make_trending_prices(int n = 60) {
    std::vector<Price> p;
    p.reserve(static_cast<std::size_t>(n));
    double v = 100.0;
    for (int i = 0; i < n; ++i) {
        v += 0.5;
        p.push_back(v);
    }
    return p;
}

TEST_CASE("WalkForwardOptimizer returns empty for insufficient price data", "[optimizer]") {
    WalkForwardOptimizer opt(100);
    const std::vector<Price> prices = {100.0, 101.0, 102.0};
    const std::vector<std::vector<double>> grid = {{2.0, 5.0}};

    auto results = opt.run(prices, 1, grid,
        [](const std::vector<double>& p) {
            return std::make_unique<MovingAverageCrossStrategy>(
                static_cast<std::size_t>(p[0]),
                static_cast<std::size_t>(p[1]));
        },
        OrderManager(5), ExecutionEngine{});

    REQUIRE(results.empty());
}

TEST_CASE("WalkForwardOptimizer returns one result per non-overlapping window", "[optimizer]") {
    // 60 prices, 20-bar windows → 3 windows
    WalkForwardOptimizer opt(20, 20);
    const auto prices = make_trending_prices(60);
    const std::vector<std::vector<double>> grid = {{2.0, 5.0}, {3.0, 6.0}};

    auto results = opt.run(prices, 1, grid,
        [](const std::vector<double>& p) {
            return std::make_unique<MovingAverageCrossStrategy>(
                static_cast<std::size_t>(p[0]),
                static_cast<std::size_t>(p[1]));
        },
        OrderManager(5), ExecutionEngine{});

    REQUIRE(results.size() == 3);
}

TEST_CASE("WalkForwardOptimizer best result has params from the grid", "[optimizer]") {
    WalkForwardOptimizer opt(30);
    const auto prices = make_trending_prices(60);
    const std::vector<std::vector<double>> grid = {{2.0, 5.0}, {3.0, 8.0}, {4.0, 10.0}};

    auto results = opt.run(prices, 1, grid,
        [](const std::vector<double>& p) {
            return std::make_unique<MovingAverageCrossStrategy>(
                static_cast<std::size_t>(p[0]),
                static_cast<std::size_t>(p[1]));
        },
        OrderManager(5), ExecutionEngine{});

    REQUIRE_FALSE(results.empty());
    for (const auto& r : results) {
        REQUIRE(r.params.size() == 2);
        REQUIRE(r.final_equity > 0.0);
    }
}

TEST_CASE("WalkForwardOptimizer works with DonchianBreakout strategy", "[optimizer]") {
    WalkForwardOptimizer opt(25);
    const auto prices = make_trending_prices(50);
    // Grid: different channel widths
    const std::vector<std::vector<double>> grid = {{5.0}, {10.0}, {15.0}};

    auto results = opt.run(prices, 1, grid,
        [](const std::vector<double>& p) {
            return std::make_unique<DonchianBreakoutStrategy>(
                static_cast<std::size_t>(p[0]));
        },
        OrderManager(3), ExecutionEngine{});

    REQUIRE_FALSE(results.empty());
    for (const auto& r : results) {
        REQUIRE(r.params.size() == 1);
    }
}
