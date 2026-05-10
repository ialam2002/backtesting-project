#include <cassert>
#include <vector>

#include "quant/analytics/performance_metrics.h"

int main() {
    using namespace quant;

    std::vector<double> returns = {0.01, -0.005, 0.02, -0.01, 0.015};

    const double sharpe = sharpe_ratio(returns);
    const double sortino = sortino_ratio(returns);
    const double vol = volatility(returns);
    const double wl = win_loss_ratio(returns);

    assert(sharpe != 0.0);
    assert(vol > 0.0);
    assert(sortino != 0.0);
    assert(wl > 0.0);

    return 0;
}
