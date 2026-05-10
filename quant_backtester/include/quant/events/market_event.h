#pragma once

#include "quant/events/event.h"

namespace quant {

struct MarketEvent : public Event {
    MarketEvent(Timestamp ts, InstrumentId instrument, Price price)
        : Event(EventType::Market, ts), instrument(instrument), price(price) {}

    InstrumentId instrument;
    Price price;
};

}  // namespace quant
