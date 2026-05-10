#include "quant/execution/execution_engine.h"

namespace quant {

FillEvent ExecutionEngine::execute(const OrderEvent& order, Price mid_price) const {
    const bool is_buy = order.side == OrderSide::Buy;
    const Price fill_price = slippage_.apply(mid_price, is_buy);
    return FillEvent(order.timestamp, order.instrument, order.side, order.qty, fill_price);
}

}  // namespace quant
