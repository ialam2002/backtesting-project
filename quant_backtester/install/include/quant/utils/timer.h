#pragma once

#include <chrono>

namespace quant {

/**
 * @brief Lightweight wall-clock timer for profiling small code paths.
 */
class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}

    /** @return Elapsed milliseconds since timer construction. */
    double elapsed_ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return static_cast<double>(diff.count()) / 1000.0;
    }

private:
    std::chrono::high_resolution_clock::time_point start_;
};

}  // namespace quant
