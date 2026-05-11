#include "quant/utils/logger.h"

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace quant {
namespace {

const char* level_name(LogLevel level) {
    // Stable enum-to-string mapping used by both file and console sinks.
    switch (level) {
        case LogLevel::Info: return "INFO";
        case LogLevel::Warn: return "WARN";
        case LogLevel::Error: return "ERROR";
    }
    return "INFO";
}

std::string utc_now_iso8601() {
    // Emit UTC timestamps to keep logs comparable across machines/time zones.
    const auto now = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm_utc{};
#if defined(_WIN32)
    gmtime_s(&tm_utc, &t);
#else
    gmtime_r(&t, &tm_utc);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_utc, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

void write_stdout(LogLevel level, const std::string& msg) {
    std::ostream& os = (level == LogLevel::Error) ? std::cerr : std::cout;
    os << '[' << level_name(level) << "] " << msg << '\n';
}

std::string escape_json(const std::string& input) {
    // Minimal escaping so each output line remains valid JSON.
    std::string out;
    out.reserve(input.size());
    for (char c : input) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

}  // namespace

StructuredLogger::StructuredLogger(const std::string& file_path, bool mirror_to_stdout)
    : mirror_to_stdout_(mirror_to_stdout) {
    // Ensure the target directory exists so first run does not fail on missing folders.
    const std::filesystem::path p(file_path);
    if (!p.parent_path().empty()) {
        std::filesystem::create_directories(p.parent_path());
    }

    out_.open(file_path, std::ios::app);
    if (!out_.is_open()) {
        throw std::runtime_error("Unable to open log file: " + file_path);
    }
}

void StructuredLogger::log(LogLevel level, const std::string& msg) {
    std::lock_guard<std::mutex> lock(mu_);
    const std::string escaped = escape_json(msg);
    // NDJSON format: one JSON object per line for easy streaming and ingestion.
    out_ << "{\"ts\":\"" << utc_now_iso8601()
         << "\",\"level\":\"" << level_name(level)
        << "\",\"msg\":\"" << escaped << "\"}" << '\n';
    out_.flush();

    if (mirror_to_stdout_) {
        write_stdout(level, msg);
    }
}

void log_info(const std::string& msg) {
    write_stdout(LogLevel::Info, msg);
}

void log_warn(const std::string& msg) {
    write_stdout(LogLevel::Warn, msg);
}

}  // namespace quant
