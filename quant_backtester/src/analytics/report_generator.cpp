#include "quant/analytics/report_generator.h"

#include <sstream>

#include "quant/analytics/performance_metrics.h"

namespace quant {

std::string build_performance_report(
    const std::vector<double>& returns,
    const std::vector<double>& equity_curve) {
    std::ostringstream out;
    out << "Performance Report\n";
    out << "------------------\n";
    out << "Sharpe: " << sharpe_ratio(returns) << "\n";
    out << "Sortino: " << sortino_ratio(returns) << "\n";
    out << "Volatility: " << volatility(returns) << "\n";
    out << "Win/Loss Ratio: " << win_loss_ratio(returns) << "\n";
    out << "Max Drawdown: " << max_drawdown(equity_curve) << "\n";
    if (equity_curve.size() >= 2) {
        const double cagr_val = cagr(
            equity_curve.front(),
            equity_curve.back(),
            static_cast<double>(equity_curve.size() - 1));
        out << "CAGR: " << cagr_val << "\n";
        out << "Calmar Ratio: " << calmar_ratio(equity_curve) << "\n";
    }
    if (!equity_curve.empty()) {
        out << "Final Equity: " << equity_curve.back() << "\n";
    }
    return out.str();
}

}  // namespace quant
