#pragma once

#include <vector>

namespace quant {

double sharpe_ratio(const std::vector<double>& returns, double annualization = 252.0);
double sortino_ratio(const std::vector<double>& returns, double annualization = 252.0);
double volatility(const std::vector<double>& returns, double annualization = 252.0);
double win_loss_ratio(const std::vector<double>& returns);
double max_drawdown(const std::vector<double>& equity_curve);

}  // namespace quant
