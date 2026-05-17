#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "quant/core/engine.h"
#include "quant/core/types.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/portfolio/portfolio.h"
#include "quant/strategy/strategy_base.h"

namespace quant {

/**
 * @brief Result produced by a single parameter combination in an optimization run.
 */
struct OptimizationResult {
    std::vector<double> params;     ///< The parameter values that were tested.
    double sharpe = 0.0;            ///< In-sample Sharpe ratio.
    double cagr = 0.0;              ///< In-sample CAGR.
    double max_drawdown = 0.0;      ///< In-sample maximum drawdown.
    double final_equity = 0.0;      ///< Final portfolio equity at end of window.
};

/**
 * @brief Walk-forward optimizer for single-instrument strategies.
 *
 * Splits the price series into consecutive in-sample windows of `in_sample_bars`
 * bars each. For every window it evaluates all parameter combinations produced by
 * the factory, ranks them by Sharpe ratio, and records the best result.
 *
 * Usage:
 * @code
 *   WalkForwardOptimizer opt(252, 5);
 *   auto results = opt.run(prices, 1, param_grid, [](const std::vector<double>& p) {
 *       return std::make_unique<MovingAverageCrossStrategy>(
 *           static_cast<std::size_t>(p[0]),
 *           static_cast<std::size_t>(p[1]));
 *   }, OrderManager(5), ExecutionEngine{}, Portfolio(50000.0));
 * @endcode
 */
class WalkForwardOptimizer {
public:
    /**
     * @param in_sample_bars  Number of bars per in-sample window.
     * @param step_bars       How many bars to advance between windows.
     *                        Defaults to in_sample_bars (non-overlapping).
     */
    explicit WalkForwardOptimizer(std::size_t in_sample_bars,
                                  std::size_t step_bars = 0);

    /**
     * @brief Run the optimizer.
     *
     * @param prices        Full price series to slice.
     * @param instrument    Instrument identifier passed to the engine.
     * @param param_grid    List of parameter vectors to evaluate per window.
     * @param strategy_factory  Callable: params → owning StrategyBase pointer.
     * @param order_manager     Copied per run (stateless between runs).
     * @param execution_engine  Copied per run.
     * @param starting_cash     Starting cash for each portfolio reset.
     *
     * @return Best OptimizationResult per window (one entry per window).
     */
    std::vector<OptimizationResult> run(
        const std::vector<Price>& prices,
        InstrumentId instrument,
        const std::vector<std::vector<double>>& param_grid,
        std::function<std::unique_ptr<StrategyBase>(const std::vector<double>&)> strategy_factory,
        OrderManager order_manager,
        ExecutionEngine execution_engine,
        double starting_cash = 100000.0) const;

private:
    std::size_t in_sample_bars_;
    std::size_t step_bars_;
};

}  // namespace quant
