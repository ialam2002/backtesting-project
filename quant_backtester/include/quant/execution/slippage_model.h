#pragma once

#include "quant/core/types.h"

namespace quant {

/**
 * @brief Applies bid/ask slippage adjustment around a mid price.
 */
class SlippageModel {
public:
    explicit SlippageModel(double bps = 1.0) : bps_(bps) {}

    /**
     * @brief Shift price up for buys and down for sells by configured basis points.
     */
    Price apply(Price mid, bool is_buy) const;

private:
    double bps_;
};

}  // namespace quant
