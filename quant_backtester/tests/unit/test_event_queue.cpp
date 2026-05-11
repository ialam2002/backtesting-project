#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <memory>

#include "quant/events/event_queue.h"
#include "quant/events/market_event.h"

using namespace quant;

TEST_CASE("EventQueue starts empty", "[event_queue]") {
    EventQueue queue;
    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}

TEST_CASE("EventQueue maintains FIFO order", "[event_queue]") {
    EventQueue queue;
    queue.push(std::make_unique<MarketEvent>(1, 1, 100.0));
    queue.push(std::make_unique<MarketEvent>(2, 1, 101.0));

    REQUIRE(queue.size() == 2);

    auto first = queue.pop();
    REQUIRE(first != nullptr);
    REQUIRE(first->timestamp == 1);

    auto second = queue.pop();
    REQUIRE(second != nullptr);
    REQUIRE(second->timestamp == 2);

    REQUIRE(queue.empty());
}

TEST_CASE("EventQueue pop returns nullptr when empty", "[event_queue]") {
    EventQueue queue;
    REQUIRE(queue.pop() == nullptr);
}
