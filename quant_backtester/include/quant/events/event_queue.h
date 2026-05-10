#pragma once

#include <queue>
#include <utility>

#include "quant/events/event.h"

namespace quant {

class EventQueue {
public:
    bool empty() const { return queue_.empty(); }
    std::size_t size() const { return queue_.size(); }

    void push(EventPtr event) { queue_.push(std::move(event)); }

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
