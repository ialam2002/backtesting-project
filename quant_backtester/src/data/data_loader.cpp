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

#ifdef QUANT_PARQUET_ENABLED
#include <arrow/api.h>
#include <parquet/arrow/reader.h>
std::vector<OhlcvRecord> load_ohlcv_from_parquet(const std::string& file_path) {
    // 1. open parquet file
    std::shared_ptr<arrow::io::ReadableFile> input_file = arrow::io::ReadableFile::Open(file_path).ValueOrDie();

    // 2. create parquet reader and read table
    std::unique_ptr<parquet::arrow::FileReader> parquet_reader;
    parquet::arrow::OpenFile(input_file, arrow::default_memory_pool(), &parquet_reader);
    std::shared_ptr<arrow::Table> table = parquet_reader->ReadTable().ValueOrDie();

    // 3. validate required columns
    std::vector<std::string> required_cols = {"timestamp", "open", "high", "low", "close", "volume"};
    for (const auto& col : required_cols) {
        if (table->GetColumnByName(col) == nullptr) {
            throw std::runtime_error("Missing required column in Parquet file: " + col);
        }
    }

    // 4. convert to OhlcvRecord vector
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
    throw std::runtime_error("Parquet support not enabled. Rebuild with QUANT_PARQUET_ENABLED defined.");
}

#endif  // QUANT_PARQUET_ENABLED
