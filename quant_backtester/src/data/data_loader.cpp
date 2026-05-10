#include "quant/data/data_loader.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace quant {
namespace {

std::vector<std::string> split_csv_row(const std::string& line) {
    std::vector<std::string> out;
    std::stringstream row(line);
    std::string cell;
    while (std::getline(row, cell, ',')) {
        out.push_back(cell);
    }
    return out;
}

std::string lower_copy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::optional<OhlcvRecord> parse_ohlcv_cells(const std::vector<std::string>& cells) {
    if (cells.size() < 6) {
        return std::nullopt;
    }

    try {
        OhlcvRecord record{};
        record.timestamp = static_cast<Timestamp>(std::stoll(cells[0]));
        record.open = std::stod(cells[1]);
        record.high = std::stod(cells[2]);
        record.low = std::stod(cells[3]);
        record.close = std::stod(cells[4]);
        record.volume = std::stod(cells[5]);
        return record;
    } catch (...) {
        return std::nullopt;
    }
}

}  // namespace

std::vector<Price> load_prices_from_csv(const std::string& file_path) {
    std::ifstream input(file_path);
    if (!input.is_open()) {
        throw std::runtime_error("Unable to open CSV file: " + file_path);
    }

    std::vector<Price> prices;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream row(line);
        std::string cell;
        if (std::getline(row, cell, ',')) {
            try {
                prices.push_back(std::stod(cell));
            } catch (...) {
                // Skip header or malformed row.
            }
        }
    }

    return prices;
}

std::vector<OhlcvRecord> load_ohlcv_from_csv(const std::string& file_path) {
    std::ifstream input(file_path);
    if (!input.is_open()) {
        throw std::runtime_error("Unable to open CSV file: " + file_path);
    }

    std::vector<OhlcvRecord> rows;
    std::string line;
    bool first_line = true;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        const auto cells = split_csv_row(line);
        if (cells.empty()) {
            continue;
        }

        if (first_line) {
            first_line = false;
            const auto first_cell = lower_copy(cells[0]);
            if (first_cell == "timestamp" || first_cell == "ts") {
                continue;
            }
        }

        auto parsed = parse_ohlcv_cells(cells);
        if (parsed.has_value()) {
            rows.push_back(*parsed);
        }
    }

    return normalize_ohlcv(std::move(rows));
}

std::vector<OhlcvRecord> load_ohlcv_from_parquet(const std::string& file_path) {
    (void)file_path;
    throw std::runtime_error("Parquet loader not implemented yet. Use CSV or add Arrow/Parquet dependency.");
}

}  // namespace quant
