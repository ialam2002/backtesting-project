#include "quant/execution/execution_engine.h"

namespace quant {

FillEvent ExecutionEngine::execute(const OrderEvent& order, Price mid_price) const {
    // Preserve old API: return the final slice when partial fills are enabled.
    auto fills = execute_with_partial_fills(order, mid_price);
    if (!fills.empty()) {
        return fills.back();
    }

    const bool is_buy = order.side == OrderSide::Buy;
    const Price fill_price = slippage_.apply(mid_price, is_buy);
    return FillEvent(order.timestamp, order.instrument, order.side, order.qty, fill_price);
}

std::vector<FillEvent> ExecutionEngine::execute_with_partial_fills(const OrderEvent& order, Price mid_price) const {
    std::vector<FillEvent> fills;
    const auto chunks = fill_simulator_.split(order.qty);
    if (chunks.empty()) {
        return fills;
    }

    const bool is_buy = order.side == OrderSide::Buy;
    // All slices use the same simulated execution price in this simplified model.
    const Price fill_price = slippage_.apply(mid_price, is_buy);
    fills.reserve(chunks.size());
    for (Quantity q : chunks) {
        fills.emplace_back(order.timestamp, order.instrument, order.side, q, fill_price);
    }
    return fills;
}

}  // namespace quant
