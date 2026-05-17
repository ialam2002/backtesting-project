#pragma once

#include <memory>
#include "quant/core/types.h"

namespace quant {

/**
 * @brief Canonical event categories used by the event-driven engine.
 */
enum class EventType {
    Market,
    Signal,
    Order,
    Fill
};

/**
 * @brief Base polymorphic event carrying type and simulation timestamp.
 */
struct Event {
    explicit Event(EventType type, Timestamp ts) : type(type), timestamp(ts) {}
    virtual ~Event() = default;

    EventType type;
    Timestamp timestamp;
};

/** Owning pointer type used for queue transport. */
using EventPtr = std::unique_ptr<Event>;

}  // namespace quant
