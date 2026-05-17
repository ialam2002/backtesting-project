#pragma once

#include <cstdint>

namespace quant {

/** Integer simulation timestamp. */
using Timestamp = std::int64_t;
/** Integer instrument identifier. */
using InstrumentId = std::int32_t;
/** Signed integer quantity in shares/contracts. */
using Quantity = std::int32_t;
/** Price scalar represented in decimal units. */
using Price = double;

}  // namespace quant
