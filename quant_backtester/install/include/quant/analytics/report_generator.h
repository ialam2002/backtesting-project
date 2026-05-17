#pragma once

#include <string>
#include <vector>

namespace quant {

/**
 * @brief Build a human-readable performance report from returns and equity data.
 */
std::string build_performance_report(
    const std::vector<double>& returns,
    const std::vector<double>& equity_curve);

}  // namespace quant
