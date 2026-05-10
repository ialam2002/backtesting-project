#pragma once

#include "quant/events/event.h"

namespace quant {

/**
 * @brief Market data update event emitted by the data feed.
 */
struct MarketEvent : public Event {
    MarketEvent(Timestamp ts, InstrumentId instrument, Price price)
        : Event(EventType::Market, ts), instrument(instrument), price(price) {}

    InstrumentId instrument;
    Price price;
};

}  // namespace quant
