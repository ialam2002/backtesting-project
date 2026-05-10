#pragma once

#include <string>
#include <vector>

#include "quant/utils/event_logger.h"

namespace quant {

class ReplayEngine {
public:
    static std::vector<LoggedEvent> read_csv(const std::string& file_path);
    static bool is_same_stream(const std::vector<LoggedEvent>& lhs, const std::vector<LoggedEvent>& rhs);
};

}  // namespace quant
