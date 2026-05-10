#include "quant/analytics/performance_metrics.h"

#include <algorithm>

namespace quant {

double max_drawdown(const std::vector<double>& equity_curve) {
    if (equity_curve.empty()) {
        return 0.0;
    }

    double peak = equity_curve.front();
    double worst_drawdown = 0.0;

    for (double e : equity_curve) {
        peak = std::max(peak, e);
        if (peak > 0.0) {
            const double dd = (peak - e) / peak;
            worst_drawdown = std::max(worst_drawdown, dd);
        }
    }

    return worst_drawdown;
}

}  // namespace quant
