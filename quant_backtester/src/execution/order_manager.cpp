#include "quant/execution/order_manager.h"

namespace quant {

std::optional<OrderEvent> OrderManager::from_signal(const SignalEvent& signal) const {
    if (!signal_handler_.accepts(signal)) {
        return std::nullopt;
    }

    auto candidate = order_generator_.generate(signal);
    if (!candidate) {
        return std::nullopt;
    }

    return order_router_.route(*candidate);
}

}  // namespace quant
