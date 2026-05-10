#include <cassert>

#include "quant/execution/order_generator.h"
#include "quant/execution/order_router.h"
#include "quant/execution/signal_handler.h"

int main() {
    using namespace quant;

    SignalHandler signal_handler;
    OrderGenerator order_generator(7);
    OrderRouter order_router;

    SignalEvent buy_signal(1, 42, SignalSide::Buy, 0.9);
    assert(signal_handler.accepts(buy_signal));

    auto generated = order_generator.generate(buy_signal);
    assert(generated.has_value());
    assert(generated->qty == 7);

    auto routed = order_router.route(*generated);
    assert(routed.has_value());
    assert(routed->instrument == 42);
    assert(routed->side == OrderSide::Buy);

    SignalEvent flat_signal(2, 42, SignalSide::Flat, 0.0);
    assert(!signal_handler.accepts(flat_signal));
    assert(!order_generator.generate(flat_signal).has_value());

    return 0;
}
