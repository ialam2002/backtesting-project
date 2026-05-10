#pragma once

#include <vector>

#include "quant/core/types.h"

namespace quant {

class DataFeed {
public:
    explicit DataFeed(std::vector<Price> prices) : prices_(std::move(prices)) {}

    bool has_next() const { return cursor_ < prices_.size(); }
    Price next();

private:
    std::vector<Price> prices_;
    std::size_t cursor_ = 0;
};

}  // namespace quant
