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

/**
 * @brief Event category stored in replay logs.
 */
enum class LoggedEventType {
    Market,
    Signal,
    Order,
    Fill
};

/**
 * @brief Flat log record for deterministic replay comparisons.
 */
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

/**
 * @brief In-memory event logger with optional CSV persistence.
 */
class EventLogger {
public:
    /** @brief Clear all captured events and reset sequence counter. */
    void clear();

    /** @brief Log a market event. */
    void log_market(const MarketEvent& market);

    /** @brief Log a signal event. */
    void log_signal(const SignalEvent& signal);

    /** @brief Log an order event. */
    void log_order(const OrderEvent& order);

    /** @brief Log a fill event. */
    void log_fill(const FillEvent& fill);

    /** @return Read-only view of logged events. */
    const std::vector<LoggedEvent>& events() const { return events_; }

    /** @brief Persist all events to CSV file. */
    bool write_csv(const std::string& file_path) const;

private:
    void push(LoggedEventType type, Timestamp ts, InstrumentId instrument, int side, Quantity qty, Price price, double strength);

    std::size_t next_sequence_ = 0;
    std::vector<LoggedEvent> events_;
};

}  // namespace quant
