#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "quant/execution/order_manager.h"
#include "quant/execution/order_generator.h"
#include "quant/execution/order_router.h"
#include "quant/execution/signal_handler.h"
#include "quant/portfolio/position_tracker.h"

using namespace quant;

TEST_CASE("SignalHandler accepts Buy and Sell, rejects Flat", "[oms]") {
    SignalHandler handler;
    REQUIRE(handler.accepts(SignalEvent(1, 42, SignalSide::Buy, 0.9)));
    REQUIRE(handler.accepts(SignalEvent(1, 42, SignalSide::Sell, 0.5)));
    REQUIRE_FALSE(handler.accepts(SignalEvent(1, 42, SignalSide::Flat, 0.0)));
}

TEST_CASE("OrderGenerator applies configured lot size", "[oms]") {
    OrderGenerator generator(7);
    auto order = generator.generate(SignalEvent(1, 42, SignalSide::Buy, 0.9));
    REQUIRE(order.has_value());
    REQUIRE(order->qty == 7);
}

TEST_CASE("OrderGenerator rejects Flat signals", "[oms]") {
    OrderGenerator generator(7);
    REQUIRE_FALSE(generator.generate(SignalEvent(1, 42, SignalSide::Flat, 0.0)).has_value());
}

TEST_CASE("OrderRouter passes Buy order through with correct fields", "[oms]") {
    OrderGenerator generator(7);
    OrderRouter router;
    auto order = generator.generate(SignalEvent(1, 42, SignalSide::Buy, 0.9));
    REQUIRE(order.has_value());
    auto routed = router.route(*order);
    REQUIRE(routed.has_value());
    REQUIRE(routed->instrument == 42);
    REQUIRE(routed->side == OrderSide::Buy);
}

TEST_CASE("OrderManager rejects order when risk limit is breached", "[oms][risk]") {
    const RiskLimits limits{5, 5, 1.0e9};
    OrderManager manager(1, limits);
    PositionTracker tracker;

    tracker.on_fill(FillEvent(1, 42, OrderSide::Buy, 5, 100.0));

    std::string reason;
    auto order = manager.from_signal(
        SignalEvent(2, 42, SignalSide::Buy, 1.0),
        tracker,
        std::unordered_map<InstrumentId, Price>{{42, 100.0}},
        &reason);

    REQUIRE_FALSE(order.has_value());
    REQUIRE(reason == "projected position exceeds max_abs_position_per_instrument");
}

TEST_CASE("OrderManager accepts order when risk limits permit", "[oms][risk]") {
    const RiskLimits limits{10, 10, 1.0e9};
    OrderManager manager(2, limits);
    PositionTracker tracker;

    auto order = manager.from_signal(
        SignalEvent(1, 7, SignalSide::Sell, 0.8),
        tracker,
        std::unordered_map<InstrumentId, Price>{{7, 50.0}},
        nullptr);

    REQUIRE(order.has_value());
    REQUIRE(order->instrument == 7);
    REQUIRE(order->qty == 2);
    REQUIRE(order->side == OrderSide::Sell);
}
