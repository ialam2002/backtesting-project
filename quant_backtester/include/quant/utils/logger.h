#pragma once

#include <fstream>
#include <mutex>
#include <string>

namespace quant {

enum class LogLevel {
    Info,
    Warn,
    Error
};

/**
 * @brief Minimal structured logger that writes timestamped lines to a file.
 */
class StructuredLogger {
public:
    StructuredLogger(const std::string& file_path, bool mirror_to_stdout = true);

    void log(LogLevel level, const std::string& msg);
    void info(const std::string& msg) { log(LogLevel::Info, msg); }
    void warn(const std::string& msg) { log(LogLevel::Warn, msg); }
    void error(const std::string& msg) { log(LogLevel::Error, msg); }

private:
    std::ofstream out_;
    bool mirror_to_stdout_;
    std::mutex mu_;
};

/** @brief Backward-compatible convenience wrappers. */
void log_info(const std::string& msg);
void log_warn(const std::string& msg);

}  // namespace quant
