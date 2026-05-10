#pragma once

#include "quant/core/types.h"

namespace quant {

/**
 * @brief Minimal monotonic simulation clock.
 */
class SimulationClock {
public:
    explicit SimulationClock(Timestamp start = 0) : now_(start) {}

    /** @return Current simulation timestamp. */
    Timestamp now() const { return now_; }

    /** @brief Advance timestamp by delta units. */
    void advance(Timestamp delta) { now_ += delta; }

    /** @brief Reset timestamp to a start value. */
    void reset(Timestamp start = 0) { now_ = start; }

private:
    Timestamp now_;
};

}  // namespace quant
