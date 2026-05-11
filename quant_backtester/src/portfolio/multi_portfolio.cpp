#include "quant/portfolio/multi_portfolio.h"

#include <algorithm>

namespace quant {

const Position MultiPortfolio::kEmptyPosition{};

double Position::unrealized_pnl(Price mark) const {
    if (qty == 0) return 0.0;
    if (qty > 0) return (mark - average_cost) * static_cast<double>(qty);
    return (average_cost - mark) * static_cast<double>(-qty);
}

MultiPortfolio::MultiPortfolio(double starting_cash)
    : cash_(starting_cash) {}

void MultiPortfolio::on_fill(const FillEvent& fill, double commission) {
    const int signed_qty = fill.side == OrderSide::Buy
                               ? static_cast<int>(fill.qty)
                               : -static_cast<int>(fill.qty);

    Position& pos = positions_[fill.instrument];

    // Realize PnL when closing or reducing an existing position.
    if (pos.qty != 0 &&
        ((pos.qty > 0 && signed_qty < 0) || (pos.qty < 0 && signed_qty > 0))) {
        const Quantity closed = static_cast<Quantity>(
            pos.qty > 0
                ? std::min(pos.qty, static_cast<Quantity>(-signed_qty))
                : std::min(static_cast<Quantity>(-pos.qty), static_cast<Quantity>(signed_qty)));

        const double pnl_per_share = pos.qty > 0
                                         ? (fill.fill_price - pos.average_cost)
                                         : (pos.average_cost - fill.fill_price);
        pos.realized_pnl += pnl_per_share * static_cast<double>(closed);
    }

    cash_ -= static_cast<double>(signed_qty) * fill.fill_price;
    cash_ -= commission;

    const Quantity new_qty = pos.qty + static_cast<Quantity>(signed_qty);

    if (new_qty == 0) {
        pos.average_cost = 0.0;
    } else if ((pos.qty >= 0 && signed_qty > 0) || (pos.qty <= 0 && signed_qty < 0)) {
        // Adding to an existing position: blend average cost.
        const double old_notional = pos.average_cost * static_cast<double>(std::abs(pos.qty));
        const double add_notional = fill.fill_price * static_cast<double>(std::abs(signed_qty));
        pos.average_cost = (old_notional + add_notional) / static_cast<double>(std::abs(new_qty));
    } else if (((pos.qty > 0 && new_qty > 0) || (pos.qty < 0 && new_qty < 0))) {
        // Partial close: keep existing average cost.
    } else {
        // Side flip: remainder opens at fill price.
        pos.average_cost = fill.fill_price;
    }

    pos.qty = new_qty;
}

const Position& MultiPortfolio::position(InstrumentId id) const {
    const auto it = positions_.find(id);
    if (it == positions_.end()) return kEmptyPosition;
    return it->second;
}

double MultiPortfolio::equity(const std::unordered_map<InstrumentId, Price>& marks) const {
    double total = cash_;
    for (const auto& [id, pos] : positions_) {
        const auto it = marks.find(id);
        if (it != marks.end()) {
            total += pos.unrealized_pnl(it->second)
                   + pos.average_cost * static_cast<double>(std::abs(pos.qty));
        }
    }
    return total;
}

double MultiPortfolio::total_realized_pnl() const {
    double total = 0.0;
    for (const auto& [id, pos] : positions_) {
        total += pos.realized_pnl;
    }
    return total;
}

}  // namespace quant
