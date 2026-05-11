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

/**
 * @brief Compound Annual Growth Rate.
 * @param start_equity  Starting portfolio value.
 * @param end_equity    Ending portfolio value.
 * @param num_periods   Number of periods elapsed (same unit as annualization).
 * @param annualization Periods per year (252 for daily, 12 for monthly).
 */
double cagr(double start_equity, double end_equity,
            double num_periods, double annualization = 252.0);

/**
 * @brief Calmar ratio: CAGR divided by maximum drawdown.
 * Returns 0 when max drawdown is zero (no drawdown ever occurred).
 */
double calmar_ratio(const std::vector<double>& equity_curve, double annualization = 252.0);

/**
 * @brief Ordinary-least-squares alpha and beta of strategy returns vs a benchmark.
 * @param returns   Strategy period returns.
 * @param benchmark Benchmark period returns (same length).
 * @param alpha     Output: annualized intercept (excess return vs benchmark).
 * @param beta      Output: slope (market sensitivity).
 */
void alpha_beta(const std::vector<double>& returns,
               const std::vector<double>& benchmark,
               double& alpha, double& beta,
               double annualization = 252.0);

}  // namespace quant
