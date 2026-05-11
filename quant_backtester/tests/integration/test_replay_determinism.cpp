#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstdio>
#include <filesystem>
#include <vector>

#include "quant/core/engine.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/portfolio/portfolio.h"
#include "quant/strategy/moving_average.h"
#include "quant/utils/event_logger.h"
#include "quant/utils/replay_engine.h"

using namespace quant;

TEST_CASE("Replay log round-trips identically after a full backtest", "[integration][replay]") {
    const std::vector<Price> prices = {100, 101, 102, 103, 104, 103, 102, 101, 100, 99, 100, 101};

    MovingAverageCrossStrategy strategy(2, 4);
    OrderManager order_manager(5);
    ExecutionEngine execution_engine(SlippageModel(1.0));
    Portfolio portfolio(50000.0);
    EventLogger logger;

    Engine engine(strategy, order_manager, execution_engine, portfolio, &logger);
    BacktestResult result = engine.run(prices, 7);

    REQUIRE_FALSE(result.event_log.empty());
    REQUIRE_FALSE(result.run_id.empty());
    REQUIRE_FALSE(result.run_directory.empty());
    REQUIRE(std::filesystem::exists(result.run_directory));
    REQUIRE_FALSE(result.replay_log_path.empty());
    REQUIRE(std::filesystem::exists(result.replay_log_path));

    const char* path = "replay_log_test.csv";
    REQUIRE(logger.write_csv(path));

    const auto replayed = ReplayEngine::read_csv(path);
    REQUIRE(ReplayEngine::is_same_stream(result.event_log, replayed));

    std::remove(path);
}
