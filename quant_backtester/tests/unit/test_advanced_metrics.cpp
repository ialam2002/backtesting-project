#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>
#include <cmath>

#include "quant/analytics/performance_metrics.h"

using namespace quant;

TEST_CASE("cagr returns zero for invalid inputs", "[analytics][cagr]") {
    REQUIRE(cagr(0.0, 100.0, 252.0) == Approx(0.0));
    REQUIRE(cagr(100.0, 200.0, 0.0) == Approx(0.0));
}

TEST_CASE("cagr for one year doubling is 100%", "[analytics][cagr]") {
    // 252 daily periods, equity doubles: CAGR should be 1.0 (100%)
    const double result = cagr(100.0, 200.0, 252.0, 252.0);
    REQUIRE(result == Approx(1.0).epsilon(1e-9));
}

TEST_CASE("cagr for flat equity is zero", "[analytics][cagr]") {
    REQUIRE(cagr(100.0, 100.0, 252.0) == Approx(0.0).margin(1e-9));
}

TEST_CASE("calmar_ratio returns zero when no drawdown", "[analytics][calmar]") {
    // Monotonically rising equity => zero drawdown => ratio is 0
    const std::vector<double> equity = {100.0, 101.0, 102.0, 103.0};
    REQUIRE(calmar_ratio(equity) == Approx(0.0));
}

TEST_CASE("calmar_ratio is positive for profitable run with drawdown", "[analytics][calmar]") {
    // Rises then falls then recovers — has both growth and a drawdown
    const std::vector<double> equity = {100.0, 110.0, 90.0, 120.0};
    const double result = calmar_ratio(equity);
    // CAGR is positive (net gain), MDD is positive => ratio > 0
    REQUIRE(result > 0.0);
}

TEST_CASE("alpha_beta outputs zero for mismatched lengths", "[analytics][alpha_beta]") {
    double a = 99.0, b = 99.0;
    alpha_beta({0.01, 0.02}, {0.01}, a, b);
    REQUIRE(a == Approx(0.0));
    REQUIRE(b == Approx(0.0));
}

TEST_CASE("beta is ~1 when strategy equals benchmark", "[analytics][alpha_beta]") {
    const std::vector<double> rets = {0.01, -0.005, 0.02, -0.01, 0.015};
    double a = 0.0, b = 0.0;
    alpha_beta(rets, rets, a, b);
    REQUIRE(b == Approx(1.0).epsilon(1e-9));
}

TEST_CASE("alpha is positive when strategy outperforms benchmark", "[analytics][alpha_beta]") {
    // Strategy consistently beats benchmark by 0.001 per period
    const std::vector<double> bench = {0.01, -0.005, 0.02, -0.01, 0.015};
    std::vector<double> strat = bench;
    for (auto& r : strat) r += 0.001;

    double a = 0.0, b = 0.0;
    alpha_beta(strat, bench, a, b);
    REQUIRE(a > 0.0);
}
