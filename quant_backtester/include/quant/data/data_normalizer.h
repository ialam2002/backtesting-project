#pragma once

#include <vector>

#include "quant/core/types.h"

namespace quant {

/**
 * @brief Canonical OHLCV row used by loader and normalizer paths.
 */
struct OhlcvRecord {
    Timestamp timestamp;
    Price open;
    Price high;
    Price low;
    Price close;
    double volume;
};

/**
 * @brief Validate a single OHLCV row for basic schema and value consistency.
 */
bool is_valid_ohlcv(const OhlcvRecord& record);

/**
 * @brief Remove invalid rows, sort by timestamp, and collapse duplicate timestamps.
 */
std::vector<OhlcvRecord> normalize_ohlcv(std::vector<OhlcvRecord> records);

}  // namespace quant
