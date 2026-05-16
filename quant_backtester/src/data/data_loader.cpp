#include "quant/data/data_loader.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <utility>

#ifdef QUANT_PARQUET_ENABLED
#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/reader.h>
#endif

namespace quant {
namespace {

std::string trim_copy(std::string value) {
    const auto not_space = [](unsigned char c) { return !std::isspace(c); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), not_space));
    value.erase(std::find_if(value.rbegin(), value.rend(), not_space).base(), value.end());
    return value;
}

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
        std::size_t consumed = 0;

        const std::string ts = trim_copy(cells[0]);
        record.timestamp = static_cast<Timestamp>(std::stoll(ts, &consumed));
        if (consumed != ts.size()) {
            return std::nullopt;
        }

        const std::string open = trim_copy(cells[1]);
        record.open = std::stod(open, &consumed);
        if (consumed != open.size()) {
            return std::nullopt;
        }

        const std::string high = trim_copy(cells[2]);
        record.high = std::stod(high, &consumed);
        if (consumed != high.size()) {
            return std::nullopt;
        }

        const std::string low = trim_copy(cells[3]);
        record.low = std::stod(low, &consumed);
        if (consumed != low.size()) {
            return std::nullopt;
        }

        const std::string close = trim_copy(cells[4]);
        record.close = std::stod(close, &consumed);
        if (consumed != close.size()) {
            return std::nullopt;
        }

        const std::string volume = trim_copy(cells[5]);
        record.volume = std::stod(volume, &consumed);
        if (consumed != volume.size()) {
            return std::nullopt;
        }

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
    bool header_skipped = false;
    std::size_t line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
        if (line.empty()) {
            continue;
        }

        std::stringstream row(line);
        std::string cell;
        if (std::getline(row, cell, ',')) {
            const std::string trimmed = trim_copy(cell);
            try {
                std::size_t consumed = 0;
                const double parsed = std::stod(trimmed, &consumed);
                if (consumed != trimmed.size()) {
                    throw std::invalid_argument("Trailing characters");
                }
                prices.push_back(parsed);
            } catch (...) {
                // Allow one non-numeric header row, then fail fast on malformed data.
                if (!header_skipped) {
                    header_skipped = true;
                    continue;
                }
                throw std::runtime_error(
                    "Malformed price row at line " + std::to_string(line_number) +
                    " in CSV file: " + file_path);
            }
        }
    }

    if (prices.empty()) {
        throw std::runtime_error("No valid price rows found in CSV file: " + file_path);
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
    std::size_t line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
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
            // Detect and skip header row if present.
            if (first_cell == "timestamp" || first_cell == "ts") {
                continue;
            }
        }

        auto parsed = parse_ohlcv_cells(cells);
        if (parsed.has_value()) {
            rows.push_back(*parsed);
        } else {
            throw std::runtime_error(
                "Malformed OHLCV row at line " + std::to_string(line_number) +
                " in CSV file: " + file_path);
        }
    }

    if (rows.empty()) {
        throw std::runtime_error("No valid OHLCV rows found in CSV file: " + file_path);
    }

    return normalize_ohlcv(std::move(rows));
}

#ifdef QUANT_PARQUET_ENABLED

std::vector<OhlcvRecord> load_ohlcv_from_parquet(const std::string& file_path) {
    // Open parquet file and load entire table into memory for now.
    std::shared_ptr<arrow::io::ReadableFile> input_file = arrow::io::ReadableFile::Open(file_path).ValueOrDie();

    // Construct parquet reader and materialize Arrow table.
    std::unique_ptr<parquet::arrow::FileReader> parquet_reader;
    parquet::arrow::OpenFile(input_file, arrow::default_memory_pool(), &parquet_reader);
    std::shared_ptr<arrow::Table> table = parquet_reader->ReadTable().ValueOrDie();

    // Validate schema contract expected by OhlcvRecord.
    std::vector<std::string> required_cols = {"timestamp", "open", "high", "low", "close", "volume"};
    for (const auto& col : required_cols) {
        if (table->GetColumnByName(col) == nullptr) {
            throw std::runtime_error("Missing required column in Parquet file: " + col);
        }
    }

    // Convert columnar arrays into row records.
    auto timestamp_array = std::static_pointer_cast<arrow::Int64Array>(table->GetColumnByName("timestamp")->chunk(0));
    auto open_array = std::static_pointer_cast<arrow::DoubleArray>(table->GetColumnByName("open")->chunk(0));
    auto high_array = std::static_pointer_cast<arrow::DoubleArray>(table->GetColumnByName("high")->chunk(0));
    auto low_array = std::static_pointer_cast<arrow::DoubleArray>(table->GetColumnByName("low")->chunk(0));
    auto close_array = std::static_pointer_cast<arrow::DoubleArray>(table->GetColumnByName("close")->chunk(0));
    auto volume_array = std::static_pointer_cast<arrow::DoubleArray>(table->GetColumnByName("volume")->chunk(0));

    std::vector<OhlcvRecord> records;
    records.reserve(table->num_rows());
    for (int64_t i = 0; i < table->num_rows(); ++i) {
        OhlcvRecord record{};
        record.timestamp = timestamp_array->Value(i);
        record.open = open_array->Value(i);
        record.high = high_array->Value(i);
        record.low = low_array->Value(i);
        record.close = close_array->Value(i);
        record.volume = volume_array->Value(i);
        records.push_back(record);
    }
    return normalize_ohlcv(std::move(records));
}

#else

std::vector<OhlcvRecord> load_ohlcv_from_parquet(const std::string& file_path) {
    (void)file_path;
    throw std::runtime_error("Parquet support not enabled. Rebuild with QUANT_PARQUET_ENABLED defined.");
}

#endif  // QUANT_PARQUET_ENABLED

}  // namespace quant
