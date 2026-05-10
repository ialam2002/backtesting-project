#include "quant/portfolio/portfolio.h"

namespace quant {

Portfolio::Portfolio(double starting_cash)
    : cash_(starting_cash), position_(0), average_cost_(0.0), realized_pnl_(0.0) {}

void Portfolio::on_fill(const FillEvent& fill, double commission) {
    const int signed_qty = fill.side == OrderSide::Buy ? fill.qty : -fill.qty;

    if (position_ != 0 && ((position_ > 0 && signed_qty < 0) || (position_ < 0 && signed_qty > 0))) {
        const Quantity closed = static_cast<Quantity>(
            (position_ > 0) ? std::min(position_, static_cast<Quantity>(-signed_qty))
                            : std::min(static_cast<Quantity>(-position_), static_cast<Quantity>(signed_qty)));

        const double pnl_per_share = (position_ > 0) ? (fill.fill_price - average_cost_)
                                                     : (average_cost_ - fill.fill_price);
        realized_pnl_ += pnl_per_share * static_cast<double>(closed);
    }

    cash_ -= static_cast<double>(signed_qty) * fill.fill_price;
    cash_ -= commission;

    const Quantity new_position = position_ + signed_qty;
    if (new_position == 0) {
        average_cost_ = 0.0;
    } else if ((position_ >= 0 && signed_qty > 0) || (position_ <= 0 && signed_qty < 0)) {
        const double old_notional = average_cost_ * static_cast<double>(std::abs(position_));
        const double add_notional = fill.fill_price * static_cast<double>(std::abs(signed_qty));
        average_cost_ = (old_notional + add_notional) / static_cast<double>(std::abs(new_position));
    } else if ((position_ > 0 && new_position > 0) || (position_ < 0 && new_position < 0)) {
        // Reduced existing position without flipping side.
    } else {
        average_cost_ = fill.fill_price;
    }

    position_ = new_position;
}

double Portfolio::unrealized_pnl(Price mark) const {
    if (position_ == 0) {
        return 0.0;
    }

    if (position_ > 0) {
        return (mark - average_cost_) * static_cast<double>(position_);
    }

    return (average_cost_ - mark) * static_cast<double>(-position_);
}

double Portfolio::equity(Price mark) const {
    return cash_ + static_cast<double>(position_) * mark;
}

}  // namespace quant
