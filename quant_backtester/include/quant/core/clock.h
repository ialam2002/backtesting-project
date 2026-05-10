#pragma once

#include "quant/core/types.h"

namespace quant {

class SimulationClock {
public:
    explicit SimulationClock(Timestamp start = 0) : now_(start) {}

    Timestamp now() const { return now_; }
    void advance(Timestamp delta) { now_ += delta; }
    void reset(Timestamp start = 0) { now_ = start; }

private:
    Timestamp now_;
};

}  // namespace quant
