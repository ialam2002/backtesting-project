#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>
#include <unordered_map>

#include "quant/portfolio/risk_manager.h"

using namespace quant;

TEST_CASE("RiskManager rejects order above max_order_qty", "[portfolio][risk]") {
    PositionTracker tracker;
    RiskManager risk(RiskLimits{5, 100, 1.0e9});

    std::string reason;
    const bool ok = risk.accepts(
        OrderEvent(1, 1, OrderSide::Buy, 6),
        tracker,
        std::unordered_map<InstrumentId, Price>{{1, 100.0}},
        &reason);

    REQUIRE_FALSE(ok);
    REQUIRE(reason == "order quantity exceeds max_order_qty");
}

TEST_CASE("RiskManager rejects projected position limit breach", "[portfolio][risk]") {
    PositionTracker tracker;
    tracker.on_fill(FillEvent(1, 1, OrderSide::Buy, 8, 100.0));

    RiskManager risk(RiskLimits{10, 10, 1.0e9});
    std::string reason;
    const bool ok = risk.accepts(
        OrderEvent(2, 1, OrderSide::Buy, 3),
        tracker,
        std::unordered_map<InstrumentId, Price>{{1, 100.0}},
        &reason);

    REQUIRE_FALSE(ok);
    REQUIRE(reason == "projected position exceeds max_abs_position_per_instrument");
}

TEST_CASE("RiskManager rejects gross notional breach", "[portfolio][risk]") {
    PositionTracker tracker;
    tracker.on_fill(FillEvent(1, 1, OrderSide::Buy, 5, 100.0));

    RiskManager risk(RiskLimits{10, 100, 700.0});
    std::string reason;
    const bool ok = risk.accepts(
        OrderEvent(2, 1, OrderSide::Buy, 3),
        tracker,
        std::unordered_map<InstrumentId, Price>{{1, 100.0}},
        &reason);

    REQUIRE_FALSE(ok);
    REQUIRE(reason == "projected gross notional exceeds max_gross_notional");
}

TEST_CASE("RiskManager accepts order within all limits", "[portfolio][risk]") {
    PositionTracker tracker;
    tracker.on_fill(FillEvent(1, 1, OrderSide::Buy, 2, 100.0));

    RiskManager risk(RiskLimits{10, 10, 1000.0});
    std::string reason;
    const bool ok = risk.accepts(
        OrderEvent(2, 1, OrderSide::Buy, 3),
        tracker,
        std::unordered_map<InstrumentId, Price>{{1, 100.0}},
        &reason);

    REQUIRE(ok);
    REQUIRE(reason.empty());
}
