#pragma once

#include <string>
#include <vector>

#include "quant/analytics/report_generator.h"
#include "quant/core/clock.h"
#include "quant/core/types.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/portfolio/portfolio.h"
#include "quant/strategy/strategy_base.h"
#include "quant/utils/event_logger.h"

namespace quant {

struct BacktestResult {
    std::vector<double> equity_curve;
    std::vector<double> returns;
    std::string report;
    std::vector<LoggedEvent> event_log;
    std::string run_id;
    std::string run_directory;
    std::string replay_log_path;
};

class Engine {
public:
    Engine(
        StrategyBase& strategy,
        OrderManager order_manager,
        ExecutionEngine execution_engine,
        Portfolio portfolio,
        EventLogger* event_logger = nullptr,
        std::string artifacts_root = "configs/experiments");

    BacktestResult run(const std::vector<Price>& prices, InstrumentId instrument = 1);

private:
    StrategyBase& strategy_;
    OrderManager order_manager_;
    ExecutionEngine execution_engine_;
    Portfolio portfolio_;
    SimulationClock clock_;
    EventLogger* event_logger_;
    std::string artifacts_root_;
};

}  // namespace quant
