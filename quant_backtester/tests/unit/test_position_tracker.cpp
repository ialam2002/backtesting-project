#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <unordered_map>

#include "quant/portfolio/position_tracker.h"

using namespace quant;

TEST_CASE("PositionTracker tracks independent instruments", "[portfolio][position_tracker]") {
    PositionTracker tracker;

    tracker.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 100.0));
    tracker.on_fill(FillEvent(2, 2, OrderSide::Sell, 5, 50.0));

    REQUIRE(tracker.position(1).qty == 10);
    REQUIRE(tracker.position(2).qty == -5);
    REQUIRE(tracker.position(1).average_cost == Approx(100.0));
    REQUIRE(tracker.position(2).average_cost == Approx(50.0));
}

TEST_CASE("PositionTracker realizes pnl on partial close", "[portfolio][position_tracker]") {
    PositionTracker tracker;

    tracker.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 100.0));
    tracker.on_fill(FillEvent(2, 1, OrderSide::Sell, 4, 110.0));

    REQUIRE(tracker.position(1).qty == 6);
    REQUIRE(tracker.position(1).realized_pnl == Approx(40.0));
    REQUIRE(tracker.total_realized_pnl() == Approx(40.0));
}

TEST_CASE("PositionTracker sets average cost to fill price on side flip", "[portfolio][position_tracker]") {
    PositionTracker tracker;

    tracker.on_fill(FillEvent(1, 1, OrderSide::Buy, 5, 100.0));
    tracker.on_fill(FillEvent(2, 1, OrderSide::Sell, 8, 90.0));

    REQUIRE(tracker.position(1).qty == -3);
    REQUIRE(tracker.position(1).average_cost == Approx(90.0));
}

TEST_CASE("PositionTracker computes unrealized and gross notional from marks", "[portfolio][position_tracker]") {
    PositionTracker tracker;

    tracker.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 100.0));
    tracker.on_fill(FillEvent(2, 2, OrderSide::Sell, 5, 50.0));

    const std::unordered_map<InstrumentId, Price> marks = {
        {1, 110.0},
        {2, 40.0},
    };

    REQUIRE(tracker.unrealized_pnl(1, 110.0) == Approx(100.0));
    REQUIRE(tracker.unrealized_pnl(2, 40.0) == Approx(50.0));
    REQUIRE(tracker.gross_notional(marks) == Approx(10.0 * 110.0 + 5.0 * 40.0));
}
