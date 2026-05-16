#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "quant/data/data_loader.h"

using namespace quant;

namespace {

std::string write_temp_csv(const std::string& file_name, const std::string& body) {
    const std::filesystem::path path = std::filesystem::temp_directory_path() / file_name;
    std::ofstream out(path);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to write temp CSV file");
    }
    out << body;
    out.close();
    return path.string();
}

}  // namespace

TEST_CASE("load_prices_from_csv parses headered one-column prices", "[data_loader]") {
    const std::string path = write_temp_csv(
        "quant_prices_valid.csv",
        "price\n"
        "100.0\n"
        "101.5\n"
        "99.25\n");

    const auto prices = load_prices_from_csv(path);

    REQUIRE(prices.size() == 3);
    REQUIRE(prices[0] == Approx(100.0));
    REQUIRE(prices[1] == Approx(101.5));
    REQUIRE(prices[2] == Approx(99.25));

    std::filesystem::remove(path);
}

TEST_CASE("load_prices_from_csv rejects malformed data row", "[data_loader]") {
    const std::string path = write_temp_csv(
        "quant_prices_invalid.csv",
        "price\n"
        "100.0\n"
        "oops\n");

    REQUIRE_THROWS_WITH(load_prices_from_csv(path), Catch::Contains("Malformed price row"));

    std::filesystem::remove(path);
}

TEST_CASE("load_prices_from_csv rejects file with no valid rows", "[data_loader]") {
    const std::string path = write_temp_csv("quant_prices_empty.csv", "header_only\n");

    REQUIRE_THROWS_WITH(load_prices_from_csv(path), Catch::Contains("No valid price rows found"));

    std::filesystem::remove(path);
}

TEST_CASE("load_ohlcv_from_csv parses canonical OHLCV schema", "[data_loader]") {
    const std::string path = write_temp_csv(
        "quant_ohlcv_valid.csv",
        "timestamp,open,high,low,close,volume\n"
        "1,100,102,99,101,1000\n"
        "2,101,103,100,102,1100\n");

    const auto rows = load_ohlcv_from_csv(path);

    REQUIRE(rows.size() == 2);
    REQUIRE(rows[0].timestamp == 1);
    REQUIRE(rows[1].close == Approx(102.0));

    std::filesystem::remove(path);
}

TEST_CASE("load_ohlcv_from_csv rejects malformed OHLCV row", "[data_loader]") {
    const std::string path = write_temp_csv(
        "quant_ohlcv_invalid.csv",
        "timestamp,open,high,low,close,volume\n"
        "1,100,102,99,101,1000\n"
        "2,not_a_number,103,100,102,1100\n");

    REQUIRE_THROWS_WITH(load_ohlcv_from_csv(path), Catch::Contains("Malformed OHLCV row"));

    std::filesystem::remove(path);
}

TEST_CASE("load_prices_from_csv throws for missing file", "[data_loader]") {
    REQUIRE_THROWS_WITH(
        load_prices_from_csv("__definitely_missing_prices_file__.csv"),
        Catch::Contains("Unable to open CSV file"));
}
