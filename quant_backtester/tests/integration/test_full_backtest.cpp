#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>

#include "quant/core/engine.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/portfolio/portfolio.h"
#include "quant/strategy/moving_average.h"

using namespace quant;

TEST_CASE("Full backtest produces equity curve with one entry per price", "[integration]") {
    const std::vector<Price> prices = {100, 101, 102, 103, 104, 103, 102, 101, 100, 99, 100, 101};

    MovingAverageCrossStrategy strategy(2, 4);
    OrderManager order_manager(5);
    ExecutionEngine execution_engine(SlippageModel(1.0));
    Portfolio portfolio(50000.0);

    Engine engine(strategy, order_manager, execution_engine, portfolio, nullptr);
    BacktestResult result = engine.run(prices, 7);

    REQUIRE_FALSE(result.equity_curve.empty());
    REQUIRE(result.equity_curve.size() == prices.size());
}

TEST_CASE("Full backtest with commission reduces equity vs zero commission", "[integration]") {
    const std::vector<Price> prices = {100, 101, 102, 103, 104, 103, 102, 101, 100, 99, 100, 101};

    MovingAverageCrossStrategy strategy_a(2, 4);
    ExecutionEngine engine_no_cost(SlippageModel(0.0));
    Engine backtest_a(strategy_a, OrderManager(5), engine_no_cost, Portfolio(50000.0), nullptr);
    const double equity_no_cost = backtest_a.run(prices, 1).equity_curve.back();

    MovingAverageCrossStrategy strategy_b(2, 4);
    ExecutionEngine engine_with_cost(SlippageModel(0.0), FillSimulator{}, TransactionCostModel(0.10));
    Engine backtest_b(strategy_b, OrderManager(5), engine_with_cost, Portfolio(50000.0), nullptr);
    const double equity_with_cost = backtest_b.run(prices, 1).equity_curve.back();

    // Commission must reduce or at-most equal equity (only if no trades fired)
    REQUIRE(equity_with_cost <= equity_no_cost);
}
