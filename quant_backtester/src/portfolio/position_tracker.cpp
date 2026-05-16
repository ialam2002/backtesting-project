#include "quant/portfolio/position_tracker.h"

#include <algorithm>
#include <cmath>

namespace quant {

const TrackedPosition PositionTracker::kEmpty{};

void PositionTracker::on_fill(const FillEvent& fill) {
	const int signed_qty = fill.side == OrderSide::Buy
							   ? static_cast<int>(fill.qty)
							   : -static_cast<int>(fill.qty);

	TrackedPosition& pos = positions_[fill.instrument];

	// Realize PnL when closing or reducing opposite-side exposure.
	if (pos.qty != 0 && ((pos.qty > 0 && signed_qty < 0) || (pos.qty < 0 && signed_qty > 0))) {
		const Quantity closed = static_cast<Quantity>(
			pos.qty > 0
				? std::min(pos.qty, static_cast<Quantity>(-signed_qty))
				: std::min(static_cast<Quantity>(-pos.qty), static_cast<Quantity>(signed_qty)));

		const double pnl_per_unit = pos.qty > 0
										? (fill.fill_price - pos.average_cost)
										: (pos.average_cost - fill.fill_price);
		pos.realized_pnl += pnl_per_unit * static_cast<double>(closed);
	}

	const Quantity new_qty = static_cast<Quantity>(pos.qty + signed_qty);

	if (new_qty == 0) {
		pos.average_cost = 0.0;
	} else if ((pos.qty >= 0 && signed_qty > 0) || (pos.qty <= 0 && signed_qty < 0)) {
		const double old_notional = pos.average_cost * static_cast<double>(std::abs(pos.qty));
		const double add_notional = fill.fill_price * static_cast<double>(std::abs(signed_qty));
		pos.average_cost = (old_notional + add_notional) / static_cast<double>(std::abs(new_qty));
	} else if ((pos.qty > 0 && new_qty > 0) || (pos.qty < 0 && new_qty < 0)) {
		// Reduced without flipping side, keep average cost.
	} else {
		// Flipped side: remaining inventory opens at current fill.
		pos.average_cost = fill.fill_price;
	}

	pos.qty = new_qty;
}

const TrackedPosition& PositionTracker::position(InstrumentId instrument) const {
	const auto it = positions_.find(instrument);
	if (it == positions_.end()) {
		return kEmpty;
	}
	return it->second;
}

double PositionTracker::total_realized_pnl() const {
	double total = 0.0;
	for (const auto& [instrument, pos] : positions_) {
		(void)instrument;
		total += pos.realized_pnl;
	}
	return total;
}

double PositionTracker::unrealized_pnl(InstrumentId instrument, Price mark) const {
	const auto& pos = position(instrument);
	if (pos.qty == 0) {
		return 0.0;
	}
	if (pos.qty > 0) {
		return (mark - pos.average_cost) * static_cast<double>(pos.qty);
	}
	return (pos.average_cost - mark) * static_cast<double>(-pos.qty);
}

double PositionTracker::gross_notional(const std::unordered_map<InstrumentId, Price>& marks) const {
	double total = 0.0;
	for (const auto& [instrument, pos] : positions_) {
		if (pos.qty == 0) {
			continue;
		}
		const auto mark_it = marks.find(instrument);
		if (mark_it == marks.end()) {
			continue;
		}
		total += std::abs(static_cast<double>(pos.qty) * mark_it->second);
	}
	return total;
}

}  // namespace quant
