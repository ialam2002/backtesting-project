#include "quant/data/data_normalizer.h"

#include <algorithm>

namespace quant {

bool is_valid_ohlcv(const OhlcvRecord& record) {
    if (record.timestamp < 0) {
        return false;
    }
    if (record.open <= 0.0 || record.high <= 0.0 || record.low <= 0.0 || record.close <= 0.0) {
        return false;
    }
    if (record.low > record.high) {
        return false;
    }
    if (record.open < record.low || record.open > record.high) {
        return false;
    }
    if (record.close < record.low || record.close > record.high) {
        return false;
    }
    if (record.volume < 0.0) {
        return false;
    }
    return true;
}

std::vector<OhlcvRecord> normalize_ohlcv(std::vector<OhlcvRecord> records) {
    records.erase(
        std::remove_if(records.begin(), records.end(), [](const OhlcvRecord& r) {
            return !is_valid_ohlcv(r);
        }),
        records.end());

    std::stable_sort(records.begin(), records.end(), [](const OhlcvRecord& lhs, const OhlcvRecord& rhs) {
        return lhs.timestamp < rhs.timestamp;
    });

    // Keep the last occurrence for duplicate timestamps after sorting.
    std::vector<OhlcvRecord> unique_records;
    unique_records.reserve(records.size());
    for (const auto& record : records) {
        if (!unique_records.empty() && unique_records.back().timestamp == record.timestamp) {
            unique_records.back() = record;
        } else {
            unique_records.push_back(record);
        }
    }

    return unique_records;
}

}  // namespace quant
