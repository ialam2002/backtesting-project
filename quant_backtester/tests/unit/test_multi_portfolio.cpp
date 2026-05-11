#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "quant/portfolio/multi_portfolio.h"

using namespace quant;

TEST_CASE("MultiPortfolio starts with correct cash and no positions", "[portfolio][multi]") {
    MultiPortfolio p(10000.0);
    REQUIRE(p.cash() == Approx(10000.0));
    REQUIRE(p.positions().empty());
    REQUIRE(p.total_realized_pnl() == Approx(0.0));
}

TEST_CASE("MultiPortfolio tracks independent positions for two instruments", "[portfolio][multi]") {
    MultiPortfolio p(50000.0);

    p.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 100.0));  // instrument 1: buy 10 @ 100
    p.on_fill(FillEvent(2, 2, OrderSide::Buy,  5, 200.0));  // instrument 2: buy 5 @ 200

    REQUIRE(p.position(1).qty == 10);
    REQUIRE(p.position(2).qty == 5);
    REQUIRE(p.cash() == Approx(50000.0 - 1000.0 - 1000.0));
}

TEST_CASE("MultiPortfolio realizes PnL correctly on a profitable sell", "[portfolio][multi]") {
    MultiPortfolio p(10000.0);
    p.on_fill(FillEvent(1, 1, OrderSide::Buy,  10, 50.0));
    p.on_fill(FillEvent(2, 1, OrderSide::Sell,  5, 60.0));

    REQUIRE(p.position(1).qty == 5);
    REQUIRE(p.position(1).realized_pnl == Approx(50.0));  // (60-50) * 5
    REQUIRE(p.total_realized_pnl() == Approx(50.0));
}

TEST_CASE("MultiPortfolio equity aggregates across instruments", "[portfolio][multi]") {
    MultiPortfolio p(10000.0);
    p.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 100.0));
    p.on_fill(FillEvent(2, 2, OrderSide::Buy,  5, 100.0));

    // Both instruments now marked at 110
    const std::unordered_map<InstrumentId, Price> marks = {{1, 110.0}, {2, 110.0}};
    const double eq = p.equity(marks);

    // Cash: 10000 - 1000 - 500 = 8500
    // Instrument 1: 10 * 110 = 1100
    // Instrument 2:  5 * 110 =  550
    REQUIRE(eq == Approx(8500.0 + 1100.0 + 550.0));
}

TEST_CASE("MultiPortfolio position() returns empty for untouched instruments", "[portfolio][multi]") {
    MultiPortfolio p(1000.0);
    const Position& pos = p.position(99);
    REQUIRE(pos.qty == 0);
    REQUIRE(pos.realized_pnl == Approx(0.0));
}

TEST_CASE("MultiPortfolio deducts commission from cash", "[portfolio][multi]") {
    MultiPortfolio p(1000.0);
    p.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 10.0), 2.50);
    REQUIRE(p.cash() == Approx(1000.0 - 100.0 - 2.50));
}
