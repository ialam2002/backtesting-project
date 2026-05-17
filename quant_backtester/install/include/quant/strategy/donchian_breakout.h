#pragma once

#include <deque>
#include <optional>

#include "quant/strategy/strategy_base.h"

namespace quant {

/**
 * @brief Donchian channel breakout strategy.
 *
 * Emits a Buy when price closes above the rolling N-period high (breakout long),
 * a Sell when price closes below the rolling N-period low (breakout short).
 * Signals are only emitted on a side change to avoid redundant orders.
 *
 * This is a classic momentum/trend-following strategy used in the original
 * Turtle Trading system.
 */
class DonchianBreakoutStrategy : public StrategyBase {
public:
    /**
     * @param window  Lookback period for high/low channel (e.g. 20 bars).
     */
    explicit DonchianBreakoutStrategy(std::size_t window);

    std::optional<SignalEvent> on_market(const MarketEvent& event) override;

private:
    std::size_t window_;
    std::deque<Price> prices_;
    SignalSide last_side_;
};

}  // namespace quant
