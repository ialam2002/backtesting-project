#include "quant/execution/order_generator.h"

namespace quant {

std::optional<OrderEvent> OrderGenerator::generate(const SignalEvent& signal) const {
    if (signal.side == SignalSide::Flat) {
        return std::nullopt;
    }

    const OrderSide side = signal.side == SignalSide::Buy ? OrderSide::Buy : OrderSide::Sell;
    return OrderEvent(signal.timestamp, signal.instrument, side, lot_size_);
}

}  // namespace quant
