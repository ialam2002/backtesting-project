#pragma once

#include "quant/events/event.h"

namespace quant {

/**
 * @brief Directional strategy output.
 */
enum class SignalSide {
    Buy,
    Sell,
    Flat
};

/**
 * @brief Event describing strategy intent before order generation.
 */
struct SignalEvent : public Event {
    SignalEvent(Timestamp ts, InstrumentId instrument, SignalSide side, double strength)
        : Event(EventType::Signal, ts), instrument(instrument), side(side), strength(strength) {}

    InstrumentId instrument;
    SignalSide side;
    double strength;
};

}  // namespace quant
