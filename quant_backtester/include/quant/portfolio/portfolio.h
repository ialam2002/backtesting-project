#pragma once

#include "quant/events/fill_event.h"

namespace quant {

class Portfolio {
public:
    explicit Portfolio(double starting_cash = 100000.0);

    void on_fill(const FillEvent& fill, double commission = 0.0);
    double cash() const { return cash_; }
    Quantity position() const { return position_; }
    double average_cost() const { return average_cost_; }
    double realized_pnl() const { return realized_pnl_; }
    double unrealized_pnl(Price mark) const;
    double equity(Price mark) const;

private:
    double cash_;
    Quantity position_;
    double average_cost_;
    double realized_pnl_;
};

}  // namespace quant
