#pragma once

#include <string>
#include <vector>

#include "quant/core/types.h"

namespace quant {

std::vector<Price> load_prices_from_csv(const std::string& file_path);

}  // namespace quant
