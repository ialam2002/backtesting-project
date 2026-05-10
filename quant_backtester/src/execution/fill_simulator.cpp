#include "quant/execution/fill_simulator.h"

#include <algorithm>
#include <cmath>

namespace quant {

std::vector<Quantity> FillSimulator::split(Quantity total_qty) const {
    std::vector<Quantity> fills;
    if (total_qty <= 0) {
        return fills;
    }

    Quantity remaining = total_qty;
    const double bounded_ratio = std::clamp(max_fill_ratio_per_slice_, 0.1, 1.0);
    const Quantity max_slice = static_cast<Quantity>(std::max(1.0, std::floor(total_qty * bounded_ratio)));

    while (remaining > 0) {
        const Quantity slice = std::min(max_slice, remaining);
        fills.push_back(slice);
        remaining -= slice;
    }

    return fills;
}

}  // namespace quant
