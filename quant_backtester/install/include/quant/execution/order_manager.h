#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "quant/execution/order_generator.h"
#include "quant/execution/order_router.h"
#include "quant/execution/signal_handler.h"
#include "quant/events/order_event.h"
#include "quant/events/signal_event.h"
#include "quant/portfolio/position_tracker.h"
#include "quant/portfolio/risk_manager.h"

namespace quant {

/**
 * @brief High-level OMS facade that runs signal validation, generation, and routing.
 */
class OrderManager {
public:
    explicit OrderManager(Quantity lot_size = 1, RiskLimits risk_limits = {})
        : signal_handler_(), order_generator_(lot_size), order_router_(), risk_manager_(risk_limits) {}

    /**
     * @brief Convert signal intent into a routable order.
     * @return Empty optional when no order should be emitted.
     */
    std::optional<OrderEvent> from_signal(const SignalEvent& signal) const;

    /**
     * @brief Convert signal into order and apply risk checks.
     * @return Empty optional when signal is rejected or violates limits.
     */
    std::optional<OrderEvent> from_signal(const SignalEvent& signal,
                                          const PositionTracker& tracker,
                                          const std::unordered_map<InstrumentId, Price>& marks,
                                          std::string* reject_reason = nullptr) const;

private:
    SignalHandler signal_handler_;
    OrderGenerator order_generator_;
    OrderRouter order_router_;
    RiskManager risk_manager_;
};

}  // namespace quant
