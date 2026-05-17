#pragma once

#include "quant/events/event.h"

namespace quant {

/**
 * @brief Order direction.
 */
enum class OrderSide {
    Buy,
    Sell
};

/**
 * @brief Event describing an executable order request.
 */
struct OrderEvent : public Event {
    OrderEvent(Timestamp ts, InstrumentId instrument, OrderSide side, Quantity qty)
        : Event(EventType::Order, ts), instrument(instrument), side(side), qty(qty) {}

    InstrumentId instrument;
    OrderSide side;
    Quantity qty;
};

}  // namespace quant
