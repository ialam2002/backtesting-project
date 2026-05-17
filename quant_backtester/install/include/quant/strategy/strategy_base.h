#pragma once

#include <optional>

#include "quant/events/market_event.h"
#include "quant/events/signal_event.h"

namespace quant {

/**
 * @brief Abstract alpha model interface.
 */
class StrategyBase {
public:
    virtual ~StrategyBase() = default;

    /**
     * @brief Process market data and optionally emit a trading signal.
     */
    virtual std::optional<SignalEvent> on_market(const MarketEvent& event) = 0;
};

}  // namespace quant
