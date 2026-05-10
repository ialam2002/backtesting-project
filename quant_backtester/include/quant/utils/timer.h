#pragma once

#include <chrono>

namespace quant {

class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    double elapsed_ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return static_cast<double>(diff.count()) / 1000.0;
    }

private:
    std::chrono::high_resolution_clock::time_point start_;
};

}  // namespace quant
