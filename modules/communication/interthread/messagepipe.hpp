#pragma once

#include <optional>
#include <thread>

#include "communication/communicator.hpp"
#include <boost/lockfree/queue.hpp>
#include <vector>

// Communication channels
namespace interthread {
using communicator::Communicator;

/**
 * MessageQueue
 * A queue which is to be used as a communication channel between TWO entities
 *
 * \note Thread-safe, DOES NOT store messages, passes on the message to
 * */
template <typename MessageType = std::vector<bool>> class MessageQueue {
    Communicator &ea, &eb; // entity a, entity b
    boost::lockfree::queue<MessageType /*, typename Options*/> _queue;

  public:
    MessageQueue(Communicator &entity_a, Communicator &entity_b,
                 size_t QUEUE_MAX_SIZE = 0)
        : ea(entity_a), eb(entity_b) {}

    /**
     * Pushes message to internal queue
     * \returns true, if push if successful, else false
     * */
    bool push(const MessageType &msg) { return _queue.push(msg); }

    /** Pop and (optionally) returns a message
     *
     * \returns message wrapped in a std::optional, if successfully popped, or
     *          std::nullopt if queue was empty
     * \note non-blocking
     * */
    std::optional<MessageType> pop() {
        MessageType m;
        if (_queue.pop(m)) {
            return m;
        } else {
            return std::nullopt;
        }
    }

    /** Pop one element, and pass it to the function provided
     *
     * \returns true, if successfully popped AND consumed message
     * */
    template <typename Functor> auto consume_one(Functor &f) {
        return _queue.consume_one(f);
    }
    /** Pop all elements SEQUENTIALLY, and pass them to the function provided
     *
     * \returns number of elements, which were successfully popped AND consumed
     * message
     * */
    template <typename Functor> auto consume_all(Functor &f) {
        return _queue.consume_all(f);
    }
};

} // namespace interthread
