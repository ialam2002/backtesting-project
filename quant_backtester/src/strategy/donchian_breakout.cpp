#include "quant/strategy/donchian_breakout.h"

#include <algorithm>
#include <limits>

namespace quant {

DonchianBreakoutStrategy::DonchianBreakoutStrategy(std::size_t window)
    : window_(window), last_side_(SignalSide::Flat) {}

std::optional<SignalEvent> DonchianBreakoutStrategy::on_market(const MarketEvent& event) {
    prices_.push_back(event.price);
    // Keep only the previous window_ prices for the channel calculation.
    // We need window_ prior bars to define the channel, then compare the new
    // price against it — so retain window_ + 1 values.
    while (prices_.size() > window_ + 1) {
        prices_.pop_front();
    }

    if (prices_.size() < window_ + 1) {
        return std::nullopt;
    }

    // Channel defined over the prior `window_` bars (excluding current bar).
    Price channel_high = std::numeric_limits<Price>::lowest();
    Price channel_low  = std::numeric_limits<Price>::max();
    for (std::size_t i = 0; i < window_; ++i) {
        channel_high = std::max(channel_high, prices_[i]);
        channel_low  = std::min(channel_low,  prices_[i]);
    }

    const Price current = prices_.back();

    SignalSide next_side = SignalSide::Flat;
    if (current > channel_high) {
        next_side = SignalSide::Buy;
    } else if (current < channel_low) {
        next_side = SignalSide::Sell;
    } else {
        return std::nullopt;
    }

    if (next_side == last_side_) {
        return std::nullopt;
    }

    last_side_ = next_side;
    return SignalEvent(event.timestamp, event.instrument, next_side, 1.0);
}

}  // namespace quant
