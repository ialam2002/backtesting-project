#include "quant/core/engine.h"

#include <memory>

#include "quant/events/market_event.h"

namespace quant {

Engine::Engine(
        StrategyBase& strategy,
        OrderManager order_manager,
        ExecutionEngine execution_engine,
        Portfolio portfolio,
        EventLogger* event_logger)
    : strategy_(strategy),
      order_manager_(order_manager),
      execution_engine_(execution_engine),
      portfolio_(portfolio),
            clock_(0),
            event_logger_(event_logger) {
        if (event_logger_ != nullptr) {
                event_logger_->clear();
        }
}

BacktestResult Engine::run(const std::vector<Price>& prices, InstrumentId instrument) {
    BacktestResult result;
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

                FillEvent fill = execution_engine_.execute(*order, p);
                if (event_logger_ != nullptr) {
                    event_logger_->log_fill(fill);
                }
                portfolio_.on_fill(fill, 0.0);
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
    }
    return result;
}

}  // namespace quant
