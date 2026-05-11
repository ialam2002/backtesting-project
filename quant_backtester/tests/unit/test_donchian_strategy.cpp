#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>

#include "quant/strategy/donchian_breakout.h"
#include "quant/events/market_event.h"

using namespace quant;

// Helper: feed a price series through the strategy and collect all signals.
static std::vector<SignalEvent> feed(DonchianBreakoutStrategy& s, const std::vector<Price>& prices, InstrumentId id = 1) {
    std::vector<SignalEvent> out;
    for (std::size_t i = 0; i < prices.size(); ++i) {
        auto sig = s.on_market(MarketEvent(static_cast<Timestamp>(i), id, prices[i]));
        if (sig) out.push_back(*sig);
    }
    return out;
}

TEST_CASE("DonchianBreakout emits no signal until window is filled", "[strategy][donchian]") {
    DonchianBreakoutStrategy s(5);
    // Feed exactly `window` bars — not enough to define channel + compare
    const std::vector<Price> prices = {10.0, 11.0, 12.0, 13.0, 14.0};
    auto sigs = feed(s, prices);
    REQUIRE(sigs.empty());
}

TEST_CASE("DonchianBreakout emits Buy on upside breakout", "[strategy][donchian]") {
    // Window = 4. Channel = [10,11,12,13]. Bar 5 = 20 > 13 → Buy
    DonchianBreakoutStrategy s(4);
    const std::vector<Price> prices = {10.0, 11.0, 12.0, 13.0, 20.0};
    auto sigs = feed(s, prices);
    REQUIRE(sigs.size() == 1);
    REQUIRE(sigs[0].side == SignalSide::Buy);
}

TEST_CASE("DonchianBreakout emits Sell on downside breakout", "[strategy][donchian]") {
    // Window = 4. Channel = [10,11,12,13]. Bar 5 = 5 < 10 → Sell
    DonchianBreakoutStrategy s(4);
    const std::vector<Price> prices = {10.0, 11.0, 12.0, 13.0, 5.0};
    auto sigs = feed(s, prices);
    REQUIRE(sigs.size() == 1);
    REQUIRE(sigs[0].side == SignalSide::Sell);
}

TEST_CASE("DonchianBreakout does not repeat the same side signal", "[strategy][donchian]") {
    DonchianBreakoutStrategy s(3);
    // Two consecutive breakout-high bars: only first triggers Buy
    const std::vector<Price> prices = {10.0, 11.0, 12.0, 20.0, 25.0};
    auto sigs = feed(s, prices);
    REQUIRE(sigs.size() == 1);
    REQUIRE(sigs[0].side == SignalSide::Buy);
}

TEST_CASE("DonchianBreakout side-flips from Buy to Sell correctly", "[strategy][donchian]") {
    DonchianBreakoutStrategy s(3);
    // Buy breakout then Sell breakout
    const std::vector<Price> prices = {10.0, 11.0, 12.0, 20.0, 19.0, 18.0, 5.0};
    auto sigs = feed(s, prices);
    REQUIRE(sigs.size() == 2);
    REQUIRE(sigs[0].side == SignalSide::Buy);
    REQUIRE(sigs[1].side == SignalSide::Sell);
}

TEST_CASE("DonchianBreakout emits no signal for flat channel price", "[strategy][donchian]") {
    DonchianBreakoutStrategy s(4);
    // Price stays within channel
    const std::vector<Price> prices = {10.0, 12.0, 11.0, 13.0, 11.5};
    auto sigs = feed(s, prices);
    REQUIRE(sigs.empty());
}
