#pragma once

#include <iostream>
#include <string>

namespace quant {

/** @brief Emit informational log line to stdout. */
inline void log_info(const std::string& msg) {
    std::cout << "[INFO] " << msg << '\n';
}

/** @brief Emit warning log line to stdout. */
inline void log_warn(const std::string& msg) {
    std::cout << "[WARN] " << msg << '\n';
}

}  // namespace quant
