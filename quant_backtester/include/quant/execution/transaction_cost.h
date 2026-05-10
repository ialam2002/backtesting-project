#pragma once

#include "quant/core/types.h"

namespace quant {

class TransactionCostModel {
public:
    explicit TransactionCostModel(double commission_per_share = 0.0)
        : commission_per_share_(commission_per_share) {}

    double commission(Quantity qty) const;

private:
    double commission_per_share_;
};

}  // namespace quant
