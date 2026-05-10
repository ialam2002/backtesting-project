#pragma once

#include <vector>

#include "quant/core/types.h"

namespace quant {

struct OhlcvRecord {
    Timestamp timestamp;
    Price open;
    Price high;
    Price low;
    Price close;
    double volume;
};

bool is_valid_ohlcv(const OhlcvRecord& record);
std::vector<OhlcvRecord> normalize_ohlcv(std::vector<OhlcvRecord> records);

}  // namespace quant
