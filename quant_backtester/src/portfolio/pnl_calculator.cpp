#include "quant/portfolio/pnl_calculator.h"

namespace quant {

PnLBreakdown PnLCalculator::compute(
	const PositionTracker& tracker,
	const std::unordered_map<InstrumentId, Price>& marks) {
	PnLBreakdown out{};
	out.realized = tracker.total_realized_pnl();

	for (const auto& [instrument, pos] : tracker.positions()) {
		const auto mark_it = marks.find(instrument);
		if (mark_it == marks.end()) {
			continue;
		}

		const double instrument_unrealized = tracker.unrealized_pnl(instrument, mark_it->second);
		out.unrealized += instrument_unrealized;

		if (pos.qty > 0) {
			out.long_unrealized += instrument_unrealized;
		} else if (pos.qty < 0) {
			out.short_unrealized += instrument_unrealized;
		}
	}

	return out;
}

std::unordered_map<InstrumentId, double> PnLCalculator::per_instrument_unrealized(
	const PositionTracker& tracker,
	const std::unordered_map<InstrumentId, Price>& marks) {
	std::unordered_map<InstrumentId, double> out;
	for (const auto& [instrument, pos] : tracker.positions()) {
		(void)pos;
		const auto mark_it = marks.find(instrument);
		if (mark_it == marks.end()) {
			continue;
		}
		out[instrument] = tracker.unrealized_pnl(instrument, mark_it->second);
	}
	return out;
}

}  // namespace quant
