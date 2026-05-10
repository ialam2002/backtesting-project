#pragma once

#include <iostream>
#include <string>

namespace quant {

inline void log_info(const std::string& msg) {
    std::cout << "[INFO] " << msg << '\n';
}

inline void log_warn(const std::string& msg) {
    std::cout << "[WARN] " << msg << '\n';
}

}  // namespace quant
