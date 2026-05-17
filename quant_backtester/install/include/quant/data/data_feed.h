#pragma once

#include <vector>

#include "quant/core/types.h"

namespace quant {

/**
 * @brief Sequential market data feeder over an in-memory price series.
 */
class DataFeed {
public:
    explicit DataFeed(std::vector<Price> prices) : prices_(std::move(prices)) {}

    /** @return True when another data point can be consumed. */
    bool has_next() const { return cursor_ < prices_.size(); }

    /** @brief Get next price or throw if feed is exhausted. */
    Price next();

private:
    std::vector<Price> prices_;
    std::size_t cursor_ = 0;
};

}  // namespace quant
