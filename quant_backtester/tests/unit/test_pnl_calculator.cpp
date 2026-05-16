#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <unordered_map>

#include "quant/portfolio/pnl_calculator.h"

using namespace quant;

TEST_CASE("PnLCalculator computes realized and unrealized totals", "[portfolio][pnl]") {
    PositionTracker tracker;

    // Instrument 1: long then partial close for realized PnL.
    tracker.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 100.0));
    tracker.on_fill(FillEvent(2, 1, OrderSide::Sell, 4, 110.0));
    // Instrument 2: open short.
    tracker.on_fill(FillEvent(3, 2, OrderSide::Sell, 5, 50.0));

    const std::unordered_map<InstrumentId, Price> marks = {
        {1, 115.0},
        {2, 45.0},
    };

    const auto breakdown = PnLCalculator::compute(tracker, marks);

    // Realized: (110 - 100) * 4 = 40
    REQUIRE(breakdown.realized == Approx(40.0));
    // Unrealized long (inst 1): (115 - 100) * 6 = 90
    // Unrealized short (inst 2): (50 - 45) * 5 = 25
    REQUIRE(breakdown.long_unrealized == Approx(90.0));
    REQUIRE(breakdown.short_unrealized == Approx(25.0));
    REQUIRE(breakdown.unrealized == Approx(115.0));
    REQUIRE(breakdown.total() == Approx(155.0));
}

TEST_CASE("PnLCalculator returns per-instrument unrealized map", "[portfolio][pnl]") {
    PositionTracker tracker;
    tracker.on_fill(FillEvent(1, 7, OrderSide::Buy, 2, 100.0));

    const std::unordered_map<InstrumentId, Price> marks = {{7, 105.0}};
    const auto per_inst = PnLCalculator::per_instrument_unrealized(tracker, marks);

    REQUIRE(per_inst.size() == 1);
    REQUIRE(per_inst.at(7) == Approx(10.0));
}
