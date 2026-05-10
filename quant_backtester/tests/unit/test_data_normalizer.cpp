#include <cassert>
#include <vector>

#include "quant/data/data_normalizer.h"

int main() {
    using namespace quant;

    std::vector<OhlcvRecord> records = {
        {3, 101, 103, 99, 100, 1000},
        {2, 100, 102, 98, 101, 1100},
        {2, 100, 102, 98, 102, 1200},
        {-1, 1, 1, 1, 1, 1},
    };

    auto normalized = normalize_ohlcv(records);

    assert(normalized.size() == 2);
    assert(normalized[0].timestamp == 2);
    assert(normalized[1].timestamp == 3);
    assert(normalized[0].close == 102);

    return 0;
}
