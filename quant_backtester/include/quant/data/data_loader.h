#pragma once

#include <string>
#include <vector>

#include "quant/core/types.h"
#include "quant/data/data_normalizer.h"

namespace quant {

std::vector<Price> load_prices_from_csv(const std::string& file_path);
std::vector<OhlcvRecord> load_ohlcv_from_csv(const std::string& file_path);

// Placeholder for future Arrow/Parquet integration.
std::vector<OhlcvRecord> load_ohlcv_from_parquet(const std::string& file_path);

}  // namespace quant
