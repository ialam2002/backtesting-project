#pragma once

#include <optional>

#include "quant/events/market_event.h"
#include "quant/events/signal_event.h"

namespace quant {

class StrategyBase {
public:
    virtual ~StrategyBase() = default;
    virtual std::optional<SignalEvent> on_market(const MarketEvent& event) = 0;
};

}  // namespace quant
