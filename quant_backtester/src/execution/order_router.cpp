#include "quant/execution/order_router.h"

namespace quant {

std::optional<OrderEvent> OrderRouter::route(const OrderEvent& order) const {
    return order;
}

}  // namespace quant
