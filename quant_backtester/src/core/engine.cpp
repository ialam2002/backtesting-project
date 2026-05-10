#include "quant/core/engine.h"

#include <filesystem>
#include <fstream>
#include <utility>

#include "quant/events/market_event.h"
#include "quant/utils/run_context.h"

namespace quant {

Engine::Engine(
    StrategyBase& strategy,
    OrderManager order_manager,
    ExecutionEngine execution_engine,
    Portfolio portfolio,
    EventLogger* event_logger,
    std::string artifacts_root)
    : strategy_(strategy),
      order_manager_(order_manager),
      execution_engine_(execution_engine),
      portfolio_(portfolio),
            clock_(0),
            event_logger_(event_logger),
            artifacts_root_(std::move(artifacts_root)) {
        if (event_logger_ != nullptr) {
                event_logger_->clear();
        }
}

BacktestResult Engine::run(const std::vector<Price>& prices, InstrumentId instrument) {
    BacktestResult result;
    // Create deterministic artifact namespace for this run.
    result.run_id = make_run_id();
    result.run_directory = ensure_run_directory(artifacts_root_, result.run_id);

    result.equity_curve.reserve(prices.size());
    result.returns.reserve(prices.size() > 0 ? prices.size() - 1 : 0);

    double prev_equity = portfolio_.cash();

    for (Price p : prices) {
        const MarketEvent market(clock_.now(), instrument, p);
        if (event_logger_ != nullptr) {
            event_logger_->log_market(market);
        }

        if (auto signal = strategy_.on_market(market)) {
            if (event_logger_ != nullptr) {
                event_logger_->log_signal(*signal);
            }

            if (auto order = order_manager_.from_signal(*signal)) {
                if (event_logger_ != nullptr) {
                    event_logger_->log_order(*order);
                }

                // Route execution through partial-fill model for realism.
                const auto fills = execution_engine_.execute_with_partial_fills(*order, p);
                for (const auto& fill : fills) {
                    if (event_logger_ != nullptr) {
                        event_logger_->log_fill(fill);
                    }
                    portfolio_.on_fill(fill, 0.0);
                }
            }
        }

        const double eq = portfolio_.equity(p);
        result.equity_curve.push_back(eq);
        if (prev_equity != 0.0) {
            result.returns.push_back((eq - prev_equity) / prev_equity);
        }
        prev_equity = eq;
        clock_.advance(1);
    }

    result.report = build_performance_report(result.returns, result.equity_curve);
    if (event_logger_ != nullptr) {
        result.event_log = event_logger_->events();
        result.replay_log_path = (std::filesystem::path(result.run_directory) / "events.csv").string();
        event_logger_->write_csv(result.replay_log_path);
    }

    // Persist text report alongside event log to keep each run self-contained.
    if (!result.run_directory.empty()) {
        const auto report_path = std::filesystem::path(result.run_directory) / "report.txt";
        std::ofstream report_file(report_path.string());
        if (report_file.is_open()) {
            report_file << result.report;
        }
    }
    return result;
}

}  // namespace quant
