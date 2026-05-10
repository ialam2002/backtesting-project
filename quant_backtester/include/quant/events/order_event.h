#pragma once

#include "quant/events/event.h"

namespace quant {

enum class OrderSide {
    Buy,
    Sell
};

struct OrderEvent : public Event {
    OrderEvent(Timestamp ts, InstrumentId instrument, OrderSide side, Quantity qty)
        : Event(EventType::Order, ts), instrument(instrument), side(side), qty(qty) {}

    InstrumentId instrument;
    OrderSide side;
    Quantity qty;
};

}  // namespace quant
