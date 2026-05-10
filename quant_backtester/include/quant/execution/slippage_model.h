#pragma once

#include "quant/core/types.h"

namespace quant {

class SlippageModel {
public:
    explicit SlippageModel(double bps = 1.0) : bps_(bps) {}
    Price apply(Price mid, bool is_buy) const;

private:
    double bps_;
};

}  // namespace quant
