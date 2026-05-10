#include "quant/strategy/moving_average.h"

#include <algorithm>

#include "quant/utils/math_helpers.h"

namespace quant {

MovingAverageCrossStrategy::MovingAverageCrossStrategy(std::size_t short_window, std::size_t long_window)
    : short_window_(short_window), long_window_(long_window), last_side_(SignalSide::Flat) {}

double MovingAverageCrossStrategy::avg(std::size_t window) const {
    return simple_average(prices_, window);
}

std::optional<SignalEvent> MovingAverageCrossStrategy::on_market(const MarketEvent& event) {
    prices_.push_back(event.price);
    while (prices_.size() > long_window_) {
        prices_.pop_front();
    }

    if (prices_.size() < long_window_) {
        return std::nullopt;
    }

    const double short_avg = avg(short_window_);
    const double long_avg = avg(long_window_);

    SignalSide next_side = SignalSide::Flat;
    if (short_avg > long_avg) {
        next_side = SignalSide::Buy;
    } else if (short_avg < long_avg) {
        next_side = SignalSide::Sell;
    }

    if (next_side == last_side_ || next_side == SignalSide::Flat) {
        return std::nullopt;
    }

    last_side_ = next_side;
    return SignalEvent(event.timestamp, event.instrument, next_side, 1.0);
}

}  // namespace quant
