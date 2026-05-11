#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "quant/execution/execution_engine.h"

using namespace quant;

TEST_CASE("Partial fills sum to total order quantity", "[execution]") {
    ExecutionEngine engine(SlippageModel(1.0), FillSimulator(0.4));
    OrderEvent order(10, 1, OrderSide::Buy, 10);

    const auto fills = engine.execute_with_partial_fills(order, 100.0);
    REQUIRE_FALSE(fills.empty());

    int total_qty = 0;
    for (const auto& fill : fills) {
        total_qty += fill.qty;
    }

    REQUIRE(total_qty == 10);
    REQUIRE(fills.size() >= 2);
}

TEST_CASE("Partial fills inherit correct instrument and side", "[execution]") {
    ExecutionEngine engine(SlippageModel(0.0), FillSimulator(0.5));
    OrderEvent order(1, 42, OrderSide::Sell, 6);

    const auto fills = engine.execute_with_partial_fills(order, 50.0);
    REQUIRE_FALSE(fills.empty());
    for (const auto& fill : fills) {
        REQUIRE(fill.instrument == 42);
        REQUIRE(fill.side == OrderSide::Sell);
    }
}

TEST_CASE("commission_for returns zero with default cost model", "[execution]") {
    ExecutionEngine engine;
    REQUIRE(engine.commission_for(100) == Approx(0.0));
}

TEST_CASE("commission_for scales with quantity", "[execution]") {
    ExecutionEngine engine(SlippageModel{}, FillSimulator{}, TransactionCostModel(0.01));
    REQUIRE(engine.commission_for(100) == Approx(1.0));
    REQUIRE(engine.commission_for(50) == Approx(0.5));
}
