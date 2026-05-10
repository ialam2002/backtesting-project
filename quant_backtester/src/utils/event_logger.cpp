#include "quant/utils/event_logger.h"

#include <fstream>
#include <iomanip>

namespace quant {
namespace {

const char* to_string(LoggedEventType type) {
    switch (type) {
        case LoggedEventType::Market:
            return "Market";
        case LoggedEventType::Signal:
            return "Signal";
        case LoggedEventType::Order:
            return "Order";
        case LoggedEventType::Fill:
            return "Fill";
    }
    return "Unknown";
}

int to_side(SignalSide side) {
    if (side == SignalSide::Buy) {
        return 1;
    }
    if (side == SignalSide::Sell) {
        return -1;
    }
    return 0;
}

int to_side(OrderSide side) {
    return side == OrderSide::Buy ? 1 : -1;
}

}  // namespace

void EventLogger::clear() {
    events_.clear();
    next_sequence_ = 0;
}

void EventLogger::push(
    LoggedEventType type,
    Timestamp ts,
    InstrumentId instrument,
    int side,
    Quantity qty,
    Price price,
    double strength) {
    // Sequence numbers preserve strict event ordering for replay checks.
    events_.push_back(LoggedEvent{next_sequence_++, type, ts, instrument, side, qty, price, strength});
}

void EventLogger::log_market(const MarketEvent& market) {
    push(LoggedEventType::Market, market.timestamp, market.instrument, 0, 0, market.price, 0.0);
}

void EventLogger::log_signal(const SignalEvent& signal) {
    push(LoggedEventType::Signal, signal.timestamp, signal.instrument, to_side(signal.side), 0, 0.0, signal.strength);
}

void EventLogger::log_order(const OrderEvent& order) {
    push(LoggedEventType::Order, order.timestamp, order.instrument, to_side(order.side), order.qty, 0.0, 0.0);
}

void EventLogger::log_fill(const FillEvent& fill) {
    push(LoggedEventType::Fill, fill.timestamp, fill.instrument, to_side(fill.side), fill.qty, fill.fill_price, 0.0);
}

bool EventLogger::write_csv(const std::string& file_path) const {
    std::ofstream out(file_path);
    if (!out.is_open()) {
        return false;
    }

    // Flat CSV is easy to diff and archive between strategy runs.
    out << "sequence,type,timestamp,instrument,side,qty,price,strength\n";
    out << std::setprecision(17);
    for (const auto& e : events_) {
        out << e.sequence << ','
            << to_string(e.type) << ','
            << e.timestamp << ','
            << e.instrument << ','
            << e.side << ','
            << e.qty << ','
            << e.price << ','
            << e.strength << '\n';
    }

    return true;
}

}  // namespace quant
