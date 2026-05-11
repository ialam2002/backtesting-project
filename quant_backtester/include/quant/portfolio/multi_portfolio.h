#pragma once

#include <unordered_map>
#include <vector>

#include "quant/core/types.h"
#include "quant/events/fill_event.h"

namespace quant {

/**
 * @brief Per-instrument position state.
 */
struct Position {
    Quantity qty = 0;
    double average_cost = 0.0;
    double realized_pnl = 0.0;

    double unrealized_pnl(Price mark) const;
};

/**
 * @brief Multi-instrument portfolio tracking cash and per-instrument positions.
 *
 * Applies fills for any number of instruments independently. The single-instrument
 * Portfolio class remains unchanged; use MultiPortfolio when running strategies
 * that trade more than one instrument.
 */
class MultiPortfolio {
public:
    explicit MultiPortfolio(double starting_cash = 100000.0);

    /**
     * @brief Apply a fill event, updating cash and the relevant instrument position.
     */
    void on_fill(const FillEvent& fill, double commission = 0.0);

    /** @return Current cash balance. */
    double cash() const { return cash_; }

    /**
     * @brief Position state for a given instrument. Returns a default (zero) Position
     *        if the instrument has never been traded.
     */
    const Position& position(InstrumentId id) const;

    /**
     * @brief Aggregate mark-to-market equity across all instruments.
     * @param marks Map of instrument id → current price for every open position.
     */
    double equity(const std::unordered_map<InstrumentId, Price>& marks) const;

    /**
     * @brief Total realized PnL summed across all instruments.
     */
    double total_realized_pnl() const;

    /** @return Read-only view of all open positions. */
    const std::unordered_map<InstrumentId, Position>& positions() const { return positions_; }

private:
    double cash_;
    std::unordered_map<InstrumentId, Position> positions_;
    static const Position kEmptyPosition;
};

}  // namespace quant
