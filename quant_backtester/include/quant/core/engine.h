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

/**
 * @brief Container for all outputs produced by a backtest run.
 */
struct BacktestResult {
    /** Portfolio equity sampled at each market step. */
    std::vector<double> equity_curve;
    /** Step-to-step returns derived from equity values. */
    std::vector<double> returns;
    /** Human-readable metrics summary. */
    std::string report;
    /** Full event stream captured during execution (if logging enabled). */
    std::vector<LoggedEvent> event_log;
    /** Unique identifier for this run. */
    std::string run_id;
    /** Directory where run artifacts are written. */
    std::string run_directory;
    /** Path to persisted event replay log. */
    std::string replay_log_path;
};

/**
 * @brief Drives end-to-end event processing for a historical backtest.
 */
class Engine {
public:
    /**
     * @brief Construct a simulation engine with pluggable strategy/execution modules.
     * @param strategy Strategy implementation that emits trading signals.
     * @param order_manager OMS pipeline turning signals into orders.
     * @param execution_engine Fill simulator and slippage model.
     * @param portfolio Portfolio state container.
     * @param event_logger Optional logger for deterministic replay artifacts.
     * @param artifacts_root Root output folder for per-run artifacts.
     */
    Engine(
        StrategyBase& strategy,
        OrderManager order_manager,
        ExecutionEngine execution_engine,
        Portfolio portfolio,
        EventLogger* event_logger = nullptr,
        std::string artifacts_root = "configs/experiments");

    /**
     * @brief Execute a full backtest pass for one instrument.
     * @param prices Ordered price series used as market input.
     * @param instrument Instrument identifier propagated through events.
     * @return Complete run artifacts and performance summary.
     */
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
