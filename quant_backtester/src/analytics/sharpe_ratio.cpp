#include "quant/analytics/performance_metrics.h"

#include <cmath>

namespace quant {
namespace {

double mean_return(const std::vector<double>& returns) {
    if (returns.empty()) {
        return 0.0;
    }

    double mean = 0.0;
    for (double r : returns) {
        mean += r;
    }
    return mean / static_cast<double>(returns.size());
}

double sample_stddev(const std::vector<double>& values, double center) {
    if (values.size() < 2) {
        return 0.0;
    }

    double variance = 0.0;
    for (double v : values) {
        const double d = v - center;
        variance += d * d;
    }
    variance /= static_cast<double>(values.size() - 1);
    return std::sqrt(variance);
}

}  // namespace

double sharpe_ratio(const std::vector<double>& returns, double annualization) {
    if (returns.size() < 2) {
        return 0.0;
    }

    const double mean = mean_return(returns);
    const double stddev = sample_stddev(returns, mean);
    if (stddev == 0.0) {
        return 0.0;
    }

    return (mean / stddev) * std::sqrt(annualization);
}

double volatility(const std::vector<double>& returns, double annualization) {
    // Volatility annualization assumes returns are sampled at uniform intervals.
    const double stddev = sample_stddev(returns, mean_return(returns));
    return stddev * std::sqrt(annualization);
}

double sortino_ratio(const std::vector<double>& returns, double annualization) {
    if (returns.empty()) {
        return 0.0;
    }

    const double mean = mean_return(returns);
    // Downside deviation uses only negative observations.
    std::vector<double> downside;
    downside.reserve(returns.size());
    for (double r : returns) {
        if (r < 0.0) {
            downside.push_back(r);
        }
    }

    if (downside.size() < 2) {
        return 0.0;
    }

    const double downside_stddev = sample_stddev(downside, 0.0);
    if (downside_stddev == 0.0) {
        return 0.0;
    }

    return (mean / downside_stddev) * std::sqrt(annualization);
}

double win_loss_ratio(const std::vector<double>& returns) {
    double avg_win = 0.0;
    double avg_loss = 0.0;
    std::size_t wins = 0;
    std::size_t losses = 0;

    for (double r : returns) {
        if (r > 0.0) {
            avg_win += r;
            ++wins;
        } else if (r < 0.0) {
            avg_loss += std::abs(r);
            ++losses;
        }
    }

    // Undefined when either bucket is empty; return 0 as neutral sentinel.
    if (wins == 0 || losses == 0) {
        return 0.0;
    }

    avg_win /= static_cast<double>(wins);
    avg_loss /= static_cast<double>(losses);
    if (avg_loss == 0.0) {
        return 0.0;
    }

    return avg_win / avg_loss;
}

}  // namespace quant
