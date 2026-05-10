#pragma once

#include "quant/events/fill_event.h"

namespace quant {

/**
 * @brief Single-instrument portfolio state with realized and unrealized PnL tracking.
 */
class Portfolio {
public:
    explicit Portfolio(double starting_cash = 100000.0);

    /**
     * @brief Apply a fill to cash/position/PnL state.
     */
    void on_fill(const FillEvent& fill, double commission = 0.0);

    /** @return Current cash balance. */
    double cash() const { return cash_; }

    /** @return Net signed position quantity. */
    Quantity position() const { return position_; }

    /** @return Average entry cost for the current open position. */
    double average_cost() const { return average_cost_; }

    /** @return Realized PnL accumulated from closed lots. */
    double realized_pnl() const { return realized_pnl_; }

    /** @return Mark-to-market unrealized PnL for the given mark price. */
    double unrealized_pnl(Price mark) const;

    /** @return Portfolio equity computed as cash + position*mark. */
    double equity(Price mark) const;

private:
    double cash_;
    Quantity position_;
    double average_cost_;
    double realized_pnl_;
};

}  // namespace quant
