#include "quant/core/walk_forward.h"

#include "quant/analytics/performance_metrics.h"

namespace quant {

WalkForwardOptimizer::WalkForwardOptimizer(std::size_t in_sample_bars, std::size_t step_bars)
    : in_sample_bars_(in_sample_bars),
      step_bars_(step_bars == 0 ? in_sample_bars : step_bars) {}

std::vector<OptimizationResult> WalkForwardOptimizer::run(
    const std::vector<Price>& prices,
    InstrumentId instrument,
    const std::vector<std::vector<double>>& param_grid,
    std::function<std::unique_ptr<StrategyBase>(const std::vector<double>&)> strategy_factory,
    OrderManager order_manager,
    ExecutionEngine execution_engine,
    double starting_cash) const {

    std::vector<OptimizationResult> window_bests;

    if (prices.size() < in_sample_bars_ || param_grid.empty()) {
        return window_bests;
    }

    for (std::size_t window_start = 0;
         window_start + in_sample_bars_ <= prices.size();
         window_start += step_bars_) {

        const std::size_t window_end = window_start + in_sample_bars_;
        const std::vector<Price> window_prices(
            prices.begin() + static_cast<std::ptrdiff_t>(window_start),
            prices.begin() + static_cast<std::ptrdiff_t>(window_end));

        OptimizationResult best;
        best.sharpe = std::numeric_limits<double>::lowest();

        for (const auto& params : param_grid) {
            auto strategy = strategy_factory(params);

            Engine engine(
                *strategy,
                order_manager,
                execution_engine,
                Portfolio(starting_cash),
                nullptr,
                "" /* no artifact output during optimization */);

            const BacktestResult result = engine.run(window_prices, instrument);

            const double s = sharpe_ratio(result.returns);
            if (s > best.sharpe) {
                best.params = params;
                best.sharpe = s;
                best.cagr = quant::cagr(
                    starting_cash,
                    result.equity_curve.empty() ? starting_cash : result.equity_curve.back(),
                    static_cast<double>(result.equity_curve.size() > 1
                                           ? result.equity_curve.size() - 1
                                           : 1));
                best.max_drawdown = max_drawdown(result.equity_curve);
                best.final_equity = result.equity_curve.empty()
                                        ? starting_cash
                                        : result.equity_curve.back();
            }
        }

        if (best.sharpe > std::numeric_limits<double>::lowest()) {
            window_bests.push_back(best);
        }
    }

    return window_bests;
}

}  // namespace quant
