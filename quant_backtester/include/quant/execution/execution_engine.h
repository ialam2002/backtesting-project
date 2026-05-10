#pragma once

#include "quant/events/fill_event.h"
#include "quant/events/order_event.h"
#include "quant/execution/slippage_model.h"

namespace quant {

class ExecutionEngine {
public:
    explicit ExecutionEngine(SlippageModel slippage = SlippageModel{}) : slippage_(slippage) {}
    FillEvent execute(const OrderEvent& order, Price mid_price) const;

private:
    SlippageModel slippage_;
};

}  // namespace quant
