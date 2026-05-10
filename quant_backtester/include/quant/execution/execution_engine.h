#pragma once

#include <vector>

#include "quant/events/fill_event.h"
#include "quant/events/order_event.h"
#include "quant/execution/fill_simulator.h"
#include "quant/execution/slippage_model.h"

namespace quant {

class ExecutionEngine {
public:
    explicit ExecutionEngine(
        SlippageModel slippage = SlippageModel{},
        FillSimulator fill_simulator = FillSimulator{})
        : slippage_(slippage), fill_simulator_(fill_simulator) {}

    FillEvent execute(const OrderEvent& order, Price mid_price) const;
    std::vector<FillEvent> execute_with_partial_fills(const OrderEvent& order, Price mid_price) const;

private:
    SlippageModel slippage_;
    FillSimulator fill_simulator_;
};

}  // namespace quant
