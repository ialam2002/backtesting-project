#pragma once

#include <vector>

#include "quant/events/fill_event.h"
#include "quant/events/order_event.h"

namespace quant {

class FillSimulator {
public:
    explicit FillSimulator(double max_fill_ratio_per_slice = 0.5)
        : max_fill_ratio_per_slice_(max_fill_ratio_per_slice) {}

    std::vector<Quantity> split(Quantity total_qty) const;

private:
    double max_fill_ratio_per_slice_;
};

}  // namespace quant
