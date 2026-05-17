#pragma once

#include <fstream>
#include <mutex>
#include <string>

namespace quant {

/** @brief Severity level encoded in structured log lines. */
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
    /**
     * @brief Create a logger that appends JSON lines to a file.
     * @param file_path Path to log file; parent directories are created automatically.
     * @param mirror_to_stdout When true, log lines are also mirrored to stdout/stderr.
     */
    StructuredLogger(const std::string& file_path, bool mirror_to_stdout = true);

    /** @brief Write one structured log entry. */
    void log(LogLevel level, const std::string& msg);
    /** @brief Convenience helper for info-level logs. */
    void info(const std::string& msg) { log(LogLevel::Info, msg); }
    /** @brief Convenience helper for warning-level logs. */
    void warn(const std::string& msg) { log(LogLevel::Warn, msg); }
    /** @brief Convenience helper for error-level logs. */
    void error(const std::string& msg) { log(LogLevel::Error, msg); }

private:
    std::ofstream out_;
    bool mirror_to_stdout_;
    std::mutex mu_;
};

/** @brief Backward-compatible convenience wrappers. */
void log_info(const std::string& msg);
/** @brief Backward-compatible warning wrapper. */
void log_warn(const std::string& msg);

}  // namespace quant
