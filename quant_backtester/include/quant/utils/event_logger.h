#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "quant/core/types.h"
#include "quant/events/fill_event.h"
#include "quant/events/market_event.h"
#include "quant/events/order_event.h"
#include "quant/events/signal_event.h"

namespace quant {

enum class LoggedEventType {
    Market,
    Signal,
    Order,
    Fill
};

struct LoggedEvent {
    std::size_t sequence;
    LoggedEventType type;
    Timestamp timestamp;
    InstrumentId instrument;
    int side;
    Quantity qty;
    Price price;
    double strength;
};

class EventLogger {
public:
    void clear();

    void log_market(const MarketEvent& market);
    void log_signal(const SignalEvent& signal);
    void log_order(const OrderEvent& order);
    void log_fill(const FillEvent& fill);

    const std::vector<LoggedEvent>& events() const { return events_; }
    bool write_csv(const std::string& file_path) const;

private:
    void push(LoggedEventType type, Timestamp ts, InstrumentId instrument, int side, Quantity qty, Price price, double strength);

    std::size_t next_sequence_ = 0;
    std::vector<LoggedEvent> events_;
};

}  // namespace quant
