#include "quant/analytics/performance_metrics.h"

#include <cmath>

namespace quant {

double sharpe_ratio(const std::vector<double>& returns, double annualization) {
    if (returns.size() < 2) {
        return 0.0;
    }

    double mean = 0.0;
    for (double r : returns) {
        mean += r;
    }
    mean /= static_cast<double>(returns.size());

    double variance = 0.0;
    for (double r : returns) {
        const double d = r - mean;
        variance += d * d;
    }
    variance /= static_cast<double>(returns.size() - 1);

    const double stddev = std::sqrt(variance);
    if (stddev == 0.0) {
        return 0.0;
    }

    return (mean / stddev) * std::sqrt(annualization);
}

}  // namespace quant
