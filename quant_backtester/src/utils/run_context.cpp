#include "quant/utils/run_context.h"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <random>
#include <sstream>

namespace quant {

std::string make_run_id() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm tm_buf{};
#ifdef _WIN32
    localtime_s(&tm_buf, &now_time);
#else
    localtime_r(&now_time, &tm_buf);
#endif

    std::mt19937 rng(static_cast<unsigned int>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count()));
    std::uniform_int_distribution<int> dist(1000, 9999);

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y%m%d_%H%M%S") << "_" << dist(rng);
    return oss.str();
}

std::string ensure_run_directory(const std::string& root_dir, const std::string& run_id) {
    const std::filesystem::path dir = std::filesystem::path(root_dir) / run_id;
    std::filesystem::create_directories(dir);
    return dir.string();
}

}  // namespace quant
