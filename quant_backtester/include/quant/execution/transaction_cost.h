#pragma once

#include "quant/core/types.h"

namespace quant {

/**
 * @brief Per-share commission model.
 */
class TransactionCostModel {
public:
    explicit TransactionCostModel(double commission_per_share = 0.0)
        : commission_per_share_(commission_per_share) {}

    /**
     * @brief Compute absolute commission for an order quantity.
     */
    double commission(Quantity qty) const;

private:
    double commission_per_share_;
};

}  // namespace quant
