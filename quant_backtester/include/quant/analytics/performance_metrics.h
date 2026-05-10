#pragma once

#include <vector>

namespace quant {

/**
 * @brief Annualized Sharpe ratio based on sample mean and sample standard deviation.
 */
double sharpe_ratio(const std::vector<double>& returns, double annualization = 252.0);

/**
 * @brief Annualized Sortino ratio using downside deviation.
 */
double sortino_ratio(const std::vector<double>& returns, double annualization = 252.0);

/**
 * @brief Annualized return volatility.
 */
double volatility(const std::vector<double>& returns, double annualization = 252.0);

/**
 * @brief Average winning return divided by average losing return magnitude.
 */
double win_loss_ratio(const std::vector<double>& returns);

/**
 * @brief Maximum peak-to-trough drawdown over an equity curve.
 */
double max_drawdown(const std::vector<double>& equity_curve);

}  // namespace quant
