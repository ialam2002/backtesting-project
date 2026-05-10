#pragma once

#include <deque>
#include <optional>

#include "quant/strategy/strategy_base.h"

namespace quant {

class MovingAverageCrossStrategy : public StrategyBase {
public:
    MovingAverageCrossStrategy(std::size_t short_window, std::size_t long_window);
    std::optional<SignalEvent> on_market(const MarketEvent& event) override;

private:
    double avg(std::size_t window) const;

    std::size_t short_window_;
    std::size_t long_window_;
    std::deque<Price> prices_;
    SignalSide last_side_;
};

}  // namespace quant
