#pragma once

#include "quant/events/event.h"
#include "quant/events/order_event.h"

namespace quant {

/**
 * @brief Event representing an executed order fill.
 */
struct FillEvent : public Event {
    FillEvent(Timestamp ts, InstrumentId instrument, OrderSide side, Quantity qty, Price fill_price)
        : Event(EventType::Fill, ts), instrument(instrument), side(side), qty(qty), fill_price(fill_price) {}

    InstrumentId instrument;
    OrderSide side;
    Quantity qty;
    Price fill_price;
};

}  // namespace quant
