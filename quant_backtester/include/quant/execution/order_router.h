#pragma once

#include <optional>

#include "quant/events/order_event.h"

namespace quant {

/**
 * @brief Final OMS stage that routes generated orders (simulation placeholder).
 */
class OrderRouter {
public:
    /** @return Routed order or empty optional when dropped. */
    std::optional<OrderEvent> route(const OrderEvent& order) const;
};

}  // namespace quant
