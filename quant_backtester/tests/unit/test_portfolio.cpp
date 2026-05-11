#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "quant/portfolio/portfolio.h"

using namespace quant;

TEST_CASE("Portfolio tracks position and cash after a buy fill", "[portfolio]") {
    Portfolio p(1000.0);
    p.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 10.0));
    REQUIRE(p.position() == 10);
    REQUIRE(p.cash() == Approx(900.0));
}

TEST_CASE("Portfolio realizes positive PnL on a profitable sell", "[portfolio]") {
    Portfolio p(1000.0);
    p.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 10.0));
    p.on_fill(FillEvent(2, 1, OrderSide::Sell, 5, 12.0));
    REQUIRE(p.position() == 5);
    REQUIRE(p.realized_pnl() > 0.0);
}

TEST_CASE("Portfolio equity is positive after profitable trades", "[portfolio]") {
    Portfolio p(1000.0);
    p.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 10.0));
    REQUIRE(p.equity(11.0) > 0.0);
}

TEST_CASE("Portfolio deducts commission from cash", "[portfolio]") {
    Portfolio p(1000.0);
    p.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 10.0), 5.0);  // $5 commission
    REQUIRE(p.cash() == Approx(895.0));  // 1000 - 100 (fill) - 5 (commission)
}
