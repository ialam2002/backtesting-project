#pragma once

#include "quant/events/event.h"

namespace quant {

enum class SignalSide {
    Buy,
    Sell,
    Flat
};

struct SignalEvent : public Event {
    SignalEvent(Timestamp ts, InstrumentId instrument, SignalSide side, double strength)
        : Event(EventType::Signal, ts), instrument(instrument), side(side), strength(strength) {}

    InstrumentId instrument;
    SignalSide side;
    double strength;
};

}  // namespace quant
