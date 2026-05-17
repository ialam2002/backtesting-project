#pragma once

#include "quant/events/signal_event.h"

namespace quant {

/**
 * @brief Pre-filter for strategy signals before order generation.
 */
class SignalHandler {
public:
    /** @return True if the signal should be processed by OMS. */
    bool accepts(const SignalEvent& signal) const;
};

}  // namespace quant
