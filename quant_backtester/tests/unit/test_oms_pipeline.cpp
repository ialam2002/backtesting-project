#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "quant/execution/order_generator.h"
#include "quant/execution/order_router.h"
#include "quant/execution/signal_handler.h"

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
