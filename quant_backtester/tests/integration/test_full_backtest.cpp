#include <cassert>
#include <vector>

#include "quant/core/engine.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/portfolio/portfolio.h"
#include "quant/strategy/moving_average.h"

int main() {
    using namespace quant;

    std::vector<Price> prices = {100, 101, 102, 103, 104, 103, 102, 101, 100, 99, 100, 101};

    MovingAverageCrossStrategy strategy(2, 4);
    OrderManager order_manager(5);
    ExecutionEngine execution_engine(SlippageModel(1.0));
    Portfolio portfolio(50000.0);

    Engine engine(strategy, order_manager, execution_engine, portfolio, nullptr);
    BacktestResult result = engine.run(prices, 7);

    assert(!result.equity_curve.empty());
    assert(result.equity_curve.size() == prices.size());

    return 0;
}
