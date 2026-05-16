#include "quant/portfolio/risk_manager.h"

#include <cmath>

namespace quant {

bool RiskManager::accepts(const OrderEvent& order,
						  const PositionTracker& tracker,
						  const std::unordered_map<InstrumentId, Price>& marks,
						  std::string* reason) const {
	auto reject = [reason](const std::string& message) {
		if (reason != nullptr) {
			*reason = message;
		}
		return false;
	};

	if (order.qty <= 0) {
		return reject("order quantity must be > 0");
	}
	if (order.instrument <= 0) {
		return reject("instrument must be > 0");
	}
	if (order.qty > limits_.max_order_qty) {
		return reject("order quantity exceeds max_order_qty");
	}

	const int signed_qty = order.side == OrderSide::Buy
							   ? static_cast<int>(order.qty)
							   : -static_cast<int>(order.qty);
	const Quantity current_qty = tracker.position(order.instrument).qty;
	const Quantity projected_qty = static_cast<Quantity>(current_qty + signed_qty);

	if (std::abs(projected_qty) > limits_.max_abs_position_per_instrument) {
		return reject("projected position exceeds max_abs_position_per_instrument");
	}

	const auto mark_it = marks.find(order.instrument);
	if (mark_it == marks.end()) {
		return reject("missing mark for instrument");
	}

	const double current_gross = tracker.gross_notional(marks);
	const double current_instr_notional = std::abs(static_cast<double>(current_qty) * mark_it->second);
	const double projected_instr_notional = std::abs(static_cast<double>(projected_qty) * mark_it->second);
	const double projected_gross = current_gross - current_instr_notional + projected_instr_notional;

	if (projected_gross > limits_.max_gross_notional) {
		return reject("projected gross notional exceeds max_gross_notional");
	}

	if (reason != nullptr) {
		reason->clear();
	}
	return true;
}

}  // namespace quant
