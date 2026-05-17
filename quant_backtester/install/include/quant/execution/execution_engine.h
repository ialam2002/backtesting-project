#pragma once

#include <vector>

#include "quant/events/fill_event.h"
#include "quant/events/order_event.h"
#include "quant/execution/fill_simulator.h"
#include "quant/execution/slippage_model.h"
#include "quant/execution/transaction_cost.h"

namespace quant {

/**
 * @brief Converts orders into one or more fill events using simulation models.
 */
class ExecutionEngine {
public:
    explicit ExecutionEngine(
        SlippageModel slippage = SlippageModel{},
        FillSimulator fill_simulator = FillSimulator{},
        TransactionCostModel cost_model = TransactionCostModel{})
        : slippage_(slippage), fill_simulator_(fill_simulator), cost_model_(cost_model) {}

    /**
     * @brief Execute an order and return the final fill (compatibility helper).
     */
    FillEvent execute(const OrderEvent& order, Price mid_price) const;

    /**
     * @brief Execute an order and return all partial fills.
     */
    std::vector<FillEvent> execute_with_partial_fills(const OrderEvent& order, Price mid_price) const;

    /**
     * @brief Compute the commission owed for a fill of the given quantity.
     */
    double commission_for(Quantity qty) const { return cost_model_.commission(qty); }

private:
    SlippageModel slippage_;
    FillSimulator fill_simulator_;
    TransactionCostModel cost_model_;
};

}  // namespace quant
