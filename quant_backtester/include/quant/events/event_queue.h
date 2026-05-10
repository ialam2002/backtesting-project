#pragma once

#include <queue>
#include <utility>

#include "quant/events/event.h"

namespace quant {

/**
 * @brief FIFO event queue used by the simple single-threaded simulation loop.
 */
class EventQueue {
public:
    /** @return True when queue has no pending events. */
    bool empty() const { return queue_.empty(); }

    /** @return Number of pending events. */
    std::size_t size() const { return queue_.size(); }

    /**
     * @brief Push an event onto the queue.
     */
    void push(EventPtr event) { queue_.push(std::move(event)); }

    /**
     * @brief Pop next event from queue.
     * @return Null pointer if queue is empty.
     */
    EventPtr pop() {
        if (queue_.empty()) {
            return nullptr;
        }
        EventPtr front = std::move(queue_.front());
        queue_.pop();
        return front;
    }

private:
    std::queue<EventPtr> queue_;
};

}  // namespace quant
