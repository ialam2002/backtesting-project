#include "quant/execution/signal_handler.h"

namespace quant {

bool SignalHandler::accepts(const SignalEvent& signal) const {
    return signal.side != SignalSide::Flat;
}

}  // namespace quant
