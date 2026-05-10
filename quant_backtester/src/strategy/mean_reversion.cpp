#include "quant/strategy/mean_reversion.h"

#include <cmath>

namespace quant {

MeanReversionStrategy::MeanReversionStrategy(std::size_t lookback, double z_threshold)
    : lookback_(lookback), z_threshold_(z_threshold) {}

std::optional<SignalEvent> MeanReversionStrategy::on_market(const MarketEvent& event) {
    prices_.push_back(event.price);
    while (prices_.size() > lookback_) {
        prices_.pop_front();
    }

    if (prices_.size() < lookback_) {
        return std::nullopt;
    }

    double mean = 0.0;
    for (double p : prices_) {
        mean += p;
    }
    mean /= static_cast<double>(prices_.size());

    double var = 0.0;
    for (double p : prices_) {
        const double d = p - mean;
        var += d * d;
    }
    var /= static_cast<double>(prices_.size());

    const double stddev = std::sqrt(var);
    if (stddev == 0.0) {
        return std::nullopt;
    }

    const double z = (event.price - mean) / stddev;
    if (z > z_threshold_) {
        return SignalEvent(event.timestamp, event.instrument, SignalSide::Sell, std::abs(z));
    }
    if (z < -z_threshold_) {
        return SignalEvent(event.timestamp, event.instrument, SignalSide::Buy, std::abs(z));
    }

    return std::nullopt;
}

}  // namespace quant
