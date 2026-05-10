#include <cassert>

#include "quant/execution/execution_engine.h"

int main() {
    using namespace quant;

    ExecutionEngine engine(SlippageModel(1.0), FillSimulator(0.4));
    OrderEvent order(10, 1, OrderSide::Buy, 10);

    const auto fills = engine.execute_with_partial_fills(order, 100.0);
    assert(!fills.empty());

    int total_qty = 0;
    for (const auto& fill : fills) {
        total_qty += fill.qty;
    }

    assert(total_qty == 10);
    assert(fills.size() >= 2);
    return 0;
}
