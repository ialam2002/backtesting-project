#pragma once

#include "quant/events/signal_event.h"

namespace quant {

class SignalHandler {
public:
    bool accepts(const SignalEvent& signal) const;
};

}  // namespace quant
