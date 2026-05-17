#pragma once

#include <string>
#include <vector>

#include "quant/core/types.h"
#include "quant/data/data_normalizer.h"

namespace quant {

/**
 * @brief Load a one-column CSV price file into a normalized price vector.
 */
std::vector<Price> load_prices_from_csv(const std::string& file_path);

/**
 * @brief Load OHLCV rows from CSV and normalize timestamp ordering/validity.
 */
std::vector<OhlcvRecord> load_ohlcv_from_csv(const std::string& file_path);

/**
 * @brief Load OHLCV rows from parquet.
 *
 * When parquet support is disabled at build time, implementations should throw
 * a runtime error with instructions to enable `QUANT_ENABLE_PARQUET`.
 */
std::vector<OhlcvRecord> load_ohlcv_from_parquet(const std::string& file_path);

}  // namespace quant
