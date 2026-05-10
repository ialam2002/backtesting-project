#include "quant/data/data_feed.h"

#include <stdexcept>

namespace quant {

Price DataFeed::next() {
    if (!has_next()) {
        throw std::out_of_range("DataFeed exhausted");
    }
    return prices_[cursor_++];
}

}  // namespace quant
