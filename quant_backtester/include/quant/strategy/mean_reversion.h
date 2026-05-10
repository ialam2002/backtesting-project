#pragma once

#include <deque>
#include <optional>

#include "quant/strategy/strategy_base.h"

namespace quant {

/**
 * @brief Z-score mean-reversion strategy over rolling lookback window.
 */
class MeanReversionStrategy : public StrategyBase {
public:
    MeanReversionStrategy(std::size_t lookback, double z_threshold);

    /** @brief Emit buy/sell signal when z-score exceeds configured threshold. */
    std::optional<SignalEvent> on_market(const MarketEvent& event) override;

private:
    std::size_t lookback_;
    double z_threshold_;
    std::deque<Price> prices_;
};

}  // namespace quant
