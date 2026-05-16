#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <optional>
#include <vector>

#include "quant/strategy/mean_reversion.h"

using namespace quant;

static std::optional<SignalEvent> feed_last_signal(MeanReversionStrategy& strategy,
                                                    const std::vector<Price>& prices,
                                                    InstrumentId instrument = 1) {
    std::optional<SignalEvent> last;
    for (std::size_t i = 0; i < prices.size(); ++i) {
        auto signal = strategy.on_market(MarketEvent(static_cast<Timestamp>(i), instrument, prices[i]));
        if (signal.has_value()) {
            last = signal;
        }
    }
    return last;
}

TEST_CASE("MeanReversion emits no signal before lookback is filled", "[strategy][mean_reversion]") {
    MeanReversionStrategy strategy(5, 1.0);
    const std::vector<Price> prices = {100.0, 101.0, 102.0, 103.0};

    auto signal = feed_last_signal(strategy, prices);
    REQUIRE_FALSE(signal.has_value());
}

TEST_CASE("MeanReversion emits Sell when z-score exceeds positive threshold", "[strategy][mean_reversion]") {
    MeanReversionStrategy strategy(5, 1.0);
    const std::vector<Price> prices = {100.0, 100.0, 100.0, 100.0, 110.0};

    auto signal = feed_last_signal(strategy, prices);
    REQUIRE(signal.has_value());
    REQUIRE(signal->side == SignalSide::Sell);
    REQUIRE(signal->strength > 1.0);
}

TEST_CASE("MeanReversion emits Buy when z-score exceeds negative threshold", "[strategy][mean_reversion]") {
    MeanReversionStrategy strategy(5, 1.0);
    const std::vector<Price> prices = {100.0, 100.0, 100.0, 100.0, 90.0};

    auto signal = feed_last_signal(strategy, prices);
    REQUIRE(signal.has_value());
    REQUIRE(signal->side == SignalSide::Buy);
    REQUIRE(signal->strength > 1.0);
}

TEST_CASE("MeanReversion emits no signal for zero-variance lookback", "[strategy][mean_reversion]") {
    MeanReversionStrategy strategy(5, 0.5);
    const std::vector<Price> prices = {100.0, 100.0, 100.0, 100.0, 100.0};

    auto signal = feed_last_signal(strategy, prices);
    REQUIRE_FALSE(signal.has_value());
}

TEST_CASE("MeanReversion rolling window eventually stops emitting after normalization", "[strategy][mean_reversion]") {
    MeanReversionStrategy strategy(5, 1.0);

    const std::vector<Price> first_block = {100.0, 100.0, 100.0, 100.0, 110.0};
    auto first_signal = feed_last_signal(strategy, first_block);
    REQUIRE(first_signal.has_value());
    REQUIRE(first_signal->side == SignalSide::Sell);

    const std::vector<Price> second_block = {100.0, 100.0, 100.0, 100.0, 100.0};
    auto second_signal = feed_last_signal(strategy, second_block);
    REQUIRE_FALSE(second_signal.has_value());
}
