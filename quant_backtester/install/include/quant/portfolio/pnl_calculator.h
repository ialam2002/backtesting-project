#pragma once

#include <unordered_map>

#include "quant/portfolio/position_tracker.h"

namespace quant {

/**
 * @brief Aggregate PnL components for a mark snapshot.
 */
struct PnLBreakdown {
    double realized = 0.0;
    double unrealized = 0.0;
    double long_unrealized = 0.0;
    double short_unrealized = 0.0;

    double total() const { return realized + unrealized; }
};

/**
 * @brief Utility methods for PnL decomposition using PositionTracker state.
 */
class PnLCalculator {
public:
    static PnLBreakdown compute(const PositionTracker& tracker,
                                const std::unordered_map<InstrumentId, Price>& marks);

    static std::unordered_map<InstrumentId, double> per_instrument_unrealized(
        const PositionTracker& tracker,
        const std::unordered_map<InstrumentId, Price>& marks);
};

}  // namespace quant
