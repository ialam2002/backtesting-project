#include <cassert>
#include <cstdio>
#include <vector>

#include "quant/core/engine.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/portfolio/portfolio.h"
#include "quant/strategy/moving_average.h"
#include "quant/utils/event_logger.h"
#include "quant/utils/replay_engine.h"

int main() {
    using namespace quant;

    std::vector<Price> prices = {100, 101, 102, 103, 104, 103, 102, 101, 100, 99, 100, 101};

    MovingAverageCrossStrategy strategy(2, 4);
    OrderManager order_manager(5);
    ExecutionEngine execution_engine(SlippageModel(1.0));
    Portfolio portfolio(50000.0);
    EventLogger logger;

    Engine engine(strategy, order_manager, execution_engine, portfolio, &logger);
    BacktestResult result = engine.run(prices, 7);

    assert(!result.event_log.empty());

    const char* path = "replay_log.csv";
    assert(logger.write_csv(path));

    const auto replayed = ReplayEngine::read_csv(path);
    assert(ReplayEngine::is_same_stream(result.event_log, replayed));

    std::remove(path);
    return 0;
}
