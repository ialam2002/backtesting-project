#pragma once

#include <string>

namespace quant {

/** @brief Generate unique run identifier for artifact grouping. */
std::string make_run_id();

/** @brief Ensure per-run directory exists and return its path. */
std::string ensure_run_directory(const std::string& root_dir, const std::string& run_id);

}  // namespace quant
