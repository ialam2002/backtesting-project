#include <iostream>
#include <vector>

#include "quant/core/engine.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/portfolio/portfolio.h"
#include "quant/strategy/moving_average.h"
#include "quant/utils/event_logger.h"

int main() {
    using namespace quant;

    std::vector<Price> demo_prices = {
        100.0, 101.0, 102.0, 101.5, 100.5, 99.5, 100.0, 101.0, 103.0, 104.0
    };

    MovingAverageCrossStrategy strategy(3, 5);
    OrderManager order_manager(10);
    ExecutionEngine execution_engine(SlippageModel(2.0));
    Portfolio portfolio(100000.0);
    EventLogger logger;

    Engine engine(strategy, order_manager, execution_engine, portfolio, &logger);
    BacktestResult result = engine.run(demo_prices, 1);

    std::cout << result.report;
    std::cout << "Run ID: " << result.run_id << '\n';
    std::cout << "Run Directory: " << result.run_directory << '\n';
    std::cout << "Replay Log: " << result.replay_log_path << '\n';
    std::cout << "Logged events: " << result.event_log.size() << '\n';
    return 0;
}
