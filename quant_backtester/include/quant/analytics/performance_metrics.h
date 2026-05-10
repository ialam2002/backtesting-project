#pragma once

#include <vector>

namespace quant {

double sharpe_ratio(const std::vector<double>& returns, double annualization = 252.0);
double max_drawdown(const std::vector<double>& equity_curve);

}  // namespace quant
