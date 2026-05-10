#pragma once

#include <optional>

#include "quant/events/order_event.h"
#include "quant/events/signal_event.h"

namespace quant {

class OrderGenerator {
public:
    explicit OrderGenerator(Quantity lot_size = 1) : lot_size_(lot_size) {}

    std::optional<OrderEvent> generate(const SignalEvent& signal) const;

private:
    Quantity lot_size_;
};

}  // namespace quant
