#include "quant/analytics/performance_metrics.h"

#include <cmath>
#include <stdexcept>

namespace quant {

double cagr(double start_equity, double end_equity,
            double num_periods, double annualization) {
    if (start_equity <= 0.0 || num_periods <= 0.0) {
        return 0.0;
    }
    const double total_return = end_equity / start_equity;
    const double years = num_periods / annualization;
    return std::pow(total_return, 1.0 / years) - 1.0;
}

double calmar_ratio(const std::vector<double>& equity_curve, double annualization) {
    if (equity_curve.size() < 2) {
        return 0.0;
    }
    const double mdd = max_drawdown(equity_curve);
    if (mdd == 0.0) {
        return 0.0;
    }
    const double annualized_cagr = cagr(
        equity_curve.front(),
        equity_curve.back(),
        static_cast<double>(equity_curve.size() - 1),
        annualization);
    return annualized_cagr / mdd;
}

void alpha_beta(const std::vector<double>& returns,
                const std::vector<double>& benchmark,
                double& alpha, double& beta,
                double annualization) {
    alpha = 0.0;
    beta = 0.0;

    if (returns.size() < 2 || returns.size() != benchmark.size()) {
        return;
    }

    const double n = static_cast<double>(returns.size());

    // Compute means
    double mean_s = 0.0, mean_b = 0.0;
    for (std::size_t i = 0; i < returns.size(); ++i) {
        mean_s += returns[i];
        mean_b += benchmark[i];
    }
    mean_s /= n;
    mean_b /= n;

    // OLS: beta = Cov(S, B) / Var(B)
    double cov_sb = 0.0, var_b = 0.0;
    for (std::size_t i = 0; i < returns.size(); ++i) {
        const double ds = returns[i] - mean_s;
        const double db = benchmark[i] - mean_b;
        cov_sb += ds * db;
        var_b  += db * db;
    }

    if (var_b == 0.0) {
        return;
    }

    beta = cov_sb / var_b;

    // Per-period alpha, then annualize
    const double alpha_period = mean_s - beta * mean_b;
    alpha = alpha_period * annualization;
}

}  // namespace quant
