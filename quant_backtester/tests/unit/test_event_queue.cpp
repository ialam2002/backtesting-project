#include <cassert>
#include <memory>

#include "quant/events/event_queue.h"
#include "quant/events/market_event.h"

int main() {
    using namespace quant;

    EventQueue queue;
    assert(queue.empty());

    queue.push(std::make_unique<MarketEvent>(1, 1, 100.0));
    queue.push(std::make_unique<MarketEvent>(2, 1, 101.0));

    assert(queue.size() == 2);

    auto first = queue.pop();
    assert(first != nullptr);
    assert(first->timestamp == 1);

    auto second = queue.pop();
    assert(second != nullptr);
    assert(second->timestamp == 2);

    assert(queue.empty());
    return 0;
}
