#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>

#include "quant/data/data_normalizer.h"

using namespace quant;

TEST_CASE("normalize_ohlcv removes invalid rows and sorts by timestamp", "[data_normalizer]") {
    std::vector<OhlcvRecord> records = {
        {3, 101, 103, 99, 100, 1000},
        {2, 100, 102, 98, 101, 1100},
        {2, 100, 102, 98, 102, 1200},
        {-1, 1, 1, 1, 1, 1},
    };

    auto normalized = normalize_ohlcv(records);

    REQUIRE(normalized.size() == 2);
    REQUIRE(normalized[0].timestamp == 2);
    REQUIRE(normalized[1].timestamp == 3);
}

TEST_CASE("normalize_ohlcv keeps last record on duplicate timestamp", "[data_normalizer]") {
    std::vector<OhlcvRecord> records = {
        {2, 100, 102, 98, 101, 1100},
        {2, 100, 102, 98, 102, 1200},
    };

    auto normalized = normalize_ohlcv(records);

    REQUIRE(normalized.size() == 1);
    REQUIRE(normalized[0].close == 102);
}

TEST_CASE("normalize_ohlcv returns empty for all-invalid input", "[data_normalizer]") {
    std::vector<OhlcvRecord> records = {{-1, 1, 1, 1, 1, 1}};
    auto normalized = normalize_ohlcv(records);
    REQUIRE(normalized.empty());
}
