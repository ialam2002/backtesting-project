#include "quant/execution/order_manager.h"

namespace quant {

std::optional<OrderEvent> OrderManager::from_signal(const SignalEvent& signal) const {
    const PositionTracker empty_tracker;
    const std::unordered_map<InstrumentId, Price> empty_marks;
    return from_signal(signal, empty_tracker, empty_marks, nullptr);
}

std::optional<OrderEvent> OrderManager::from_signal(
    const SignalEvent& signal,
    const PositionTracker& tracker,
    const std::unordered_map<InstrumentId, Price>& marks,
    std::string* reject_reason) const {
    if (!signal_handler_.accepts(signal)) {
        if (reject_reason != nullptr) {
            *reject_reason = "signal rejected by handler";
        }
        return std::nullopt;
    }

    auto candidate = order_generator_.generate(signal);
    if (!candidate) {
        if (reject_reason != nullptr) {
            *reject_reason = "order generation returned empty";
        }
        return std::nullopt;
    }

    auto routed = order_router_.route(*candidate);
    if (!routed) {
        if (reject_reason != nullptr) {
            *reject_reason = "order routing returned empty";
        }
        return std::nullopt;
    }

    std::string risk_reason;
    if (!risk_manager_.accepts(*routed, tracker, marks, &risk_reason)) {
        if (reject_reason != nullptr) {
            *reject_reason = risk_reason;
        }
        return std::nullopt;
    }

    if (reject_reason != nullptr) {
        reject_reason->clear();
    }
    return routed;
}

}  // namespace quant
