#pragma once

#include <string>
#include <vector>

#include "quant/utils/event_logger.h"

namespace quant {

/**
 * @brief Utilities for loading and comparing persisted event streams.
 */
class ReplayEngine {
public:
    /** @brief Read logged events from CSV file. */
    static std::vector<LoggedEvent> read_csv(const std::string& file_path);

    /** @brief Compare two event streams for deterministic equivalence. */
    static bool is_same_stream(const std::vector<LoggedEvent>& lhs, const std::vector<LoggedEvent>& rhs);
};

}  // namespace quant
