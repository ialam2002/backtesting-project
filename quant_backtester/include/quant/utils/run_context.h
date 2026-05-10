#pragma once

#include <string>

namespace quant {

std::string make_run_id();
std::string ensure_run_directory(const std::string& root_dir, const std::string& run_id);

}  // namespace quant
