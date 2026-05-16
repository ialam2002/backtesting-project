#pragma once

#include <string>
#include <unordered_map>

#include "quant/events/order_event.h"
#include "quant/portfolio/position_tracker.h"

namespace quant {

/**
 * @brief Hard risk limits for validating candidate orders.
 */
struct RiskLimits {
    Quantity max_order_qty = 1000000;
    Quantity max_abs_position_per_instrument = 1000000;
    double max_gross_notional = 1.0e15;
};

/**
 * @brief Stateless risk checks using current positions and market marks.
 */
class RiskManager {
public:
    explicit RiskManager(RiskLimits limits = {}) : limits_(limits) {}

    /**
     * @brief Validate whether an order can be accepted under configured limits.
     * @param reason Optional out param populated on rejection.
     */
    bool accepts(const OrderEvent& order,
                 const PositionTracker& tracker,
                 const std::unordered_map<InstrumentId, Price>& marks,
                 std::string* reason = nullptr) const;

private:
    RiskLimits limits_;
};

}  // namespace quant
