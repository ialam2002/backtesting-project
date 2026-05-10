#include "quant/utils/replay_engine.h"

#include <fstream>
#include <sstream>

namespace quant {
namespace {

LoggedEventType parse_type(const std::string& value) {
    if (value == "Market") {
        return LoggedEventType::Market;
    }
    if (value == "Signal") {
        return LoggedEventType::Signal;
    }
    if (value == "Order") {
        return LoggedEventType::Order;
    }
    return LoggedEventType::Fill;
}

std::vector<std::string> split_csv(const std::string& line) {
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ',')) {
        out.push_back(cell);
    }
    return out;
}

}  // namespace

std::vector<LoggedEvent> ReplayEngine::read_csv(const std::string& file_path) {
    std::ifstream in(file_path);
    std::vector<LoggedEvent> out;
    if (!in.is_open()) {
        return out;
    }

    std::string line;
    if (!std::getline(in, line)) {
        return out;
    }

    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        const auto cells = split_csv(line);
        if (cells.size() != 8) {
            continue;
        }

        LoggedEvent e{};
        e.sequence = static_cast<std::size_t>(std::stoull(cells[0]));
        e.type = parse_type(cells[1]);
        e.timestamp = static_cast<Timestamp>(std::stoll(cells[2]));
        e.instrument = static_cast<InstrumentId>(std::stoi(cells[3]));
        e.side = std::stoi(cells[4]);
        e.qty = static_cast<Quantity>(std::stoi(cells[5]));
        e.price = std::stod(cells[6]);
        e.strength = std::stod(cells[7]);
        out.push_back(e);
    }

    return out;
}

bool ReplayEngine::is_same_stream(const std::vector<LoggedEvent>& lhs, const std::vector<LoggedEvent>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (std::size_t i = 0; i < lhs.size(); ++i) {
        const LoggedEvent& a = lhs[i];
        const LoggedEvent& b = rhs[i];
        if (a.sequence != b.sequence ||
            a.type != b.type ||
            a.timestamp != b.timestamp ||
            a.instrument != b.instrument ||
            a.side != b.side ||
            a.qty != b.qty ||
            a.price != b.price ||
            a.strength != b.strength) {
            return false;
        }
    }

    return true;
}

}  // namespace quant
