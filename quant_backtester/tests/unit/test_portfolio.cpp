#include <cassert>

#include "quant/portfolio/portfolio.h"

int main() {
    using namespace quant;

    Portfolio p(1000.0);

    p.on_fill(FillEvent(1, 1, OrderSide::Buy, 10, 10.0));
    assert(p.position() == 10);
    assert(p.cash() == 900.0);

    p.on_fill(FillEvent(2, 1, OrderSide::Sell, 5, 12.0));
    assert(p.position() == 5);
    assert(p.realized_pnl() > 0.0);

    const double eq = p.equity(11.0);
    assert(eq > 0.0);

    return 0;
}
