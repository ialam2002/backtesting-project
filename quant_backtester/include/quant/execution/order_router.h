#pragma once

#include <optional>

#include "quant/events/order_event.h"

namespace quant {

class OrderRouter {
public:
    std::optional<OrderEvent> route(const OrderEvent& order) const;
};

}  // namespace quant
