#pragma once

#include <unordered_map>

#include "quant/events/fill_event.h"

namespace quant {

/**
 * @brief Per-instrument position state tracked from fills.
 */
struct TrackedPosition {
    Quantity qty = 0;
    double average_cost = 0.0;
    double realized_pnl = 0.0;
};

/**
 * @brief Tracks multi-instrument quantities, average cost, and realized PnL.
 */
class PositionTracker {
public:
    /** @brief Apply an execution fill to instrument state. */
    void on_fill(const FillEvent& fill);

    /** @brief Read-only view of all tracked positions. */
    const std::unordered_map<InstrumentId, TrackedPosition>& positions() const { return positions_; }

    /** @brief Return position for instrument, or zeroed default when unseen. */
    const TrackedPosition& position(InstrumentId instrument) const;

    /** @brief Sum realized PnL across all instruments. */
    double total_realized_pnl() const;

    /** @brief Compute mark-to-market unrealized PnL for one instrument. */
    double unrealized_pnl(InstrumentId instrument, Price mark) const;

    /** @brief Compute gross notional using provided marks and absolute quantities. */
    double gross_notional(const std::unordered_map<InstrumentId, Price>& marks) const;

private:
    std::unordered_map<InstrumentId, TrackedPosition> positions_;
    static const TrackedPosition kEmpty;
};

}  // namespace quant
