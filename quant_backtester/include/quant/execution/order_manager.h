#pragma once

#include <optional>

#include "quant/execution/order_generator.h"
#include "quant/execution/order_router.h"
#include "quant/execution/signal_handler.h"
#include "quant/events/order_event.h"
#include "quant/events/signal_event.h"

namespace quant {

/**
 * @brief High-level OMS facade that runs signal validation, generation, and routing.
 */
class OrderManager {
public:
    explicit OrderManager(Quantity lot_size = 1)
        : signal_handler_(), order_generator_(lot_size), order_router_() {}

    /**
     * @brief Convert signal intent into a routable order.
     * @return Empty optional when no order should be emitted.
     */
    std::optional<OrderEvent> from_signal(const SignalEvent& signal) const;

private:
    SignalHandler signal_handler_;
    OrderGenerator order_generator_;
    OrderRouter order_router_;
};

}  // namespace quant
