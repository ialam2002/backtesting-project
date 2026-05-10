#pragma once

#include <memory>
#include "quant/core/types.h"

namespace quant {

enum class EventType {
    Market,
    Signal,
    Order,
    Fill
};

struct Event {
    explicit Event(EventType type, Timestamp ts) : type(type), timestamp(ts) {}
    virtual ~Event() = default;

    EventType type;
    Timestamp timestamp;
};

using EventPtr = std::unique_ptr<Event>;

}  // namespace quant
