#include "quant/execution/slippage_model.h"

namespace quant {

Price SlippageModel::apply(Price mid, bool is_buy) const {
    const double shift = mid * (bps_ / 10000.0);
    return is_buy ? (mid + shift) : (mid - shift);
}

}  // namespace quant
