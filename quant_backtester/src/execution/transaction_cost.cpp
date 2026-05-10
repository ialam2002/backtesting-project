#include "quant/execution/transaction_cost.h"

#include <cmath>

namespace quant {

double TransactionCostModel::commission(Quantity qty) const {
    return commission_per_share_ * static_cast<double>(std::abs(qty));
}

}  // namespace quant
