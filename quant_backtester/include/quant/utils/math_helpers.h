#pragma once

#include <cstddef>
#include <deque>

namespace quant {

inline double simple_average(const std::deque<double>& values, std::size_t window) {
    if (values.empty() || window == 0 || values.size() < window) {
        return 0.0;
    }

    double sum = 0.0;
    for (std::size_t i = values.size() - window; i < values.size(); ++i) {
        sum += values[i];
    }
    return sum / static_cast<double>(window);
}

}  // namespace quant
