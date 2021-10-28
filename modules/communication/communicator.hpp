#pragma once

#include "id_creator.hpp"
import <memory>;
import <optional>;

namespace communicator {
/**
 * \note This MessageChannelType is currently BinaryChannel only (two entities)
 * */
template <typename MessageChannelType> class Communicator : _ID {
    using MessageChannelPtr = std::shared_ptr<MessageChannelType>;

    MessageChannelPtr channel;

  private:
    // This way, we can only have a channel, if we request the other person
    // too :D...
    // The idea is this:
    //
    // { } (pair?) (🙏){ }
    //  |-/          \--|
    // /\              /\
        //
    // called by the requester
    MessageChannelPtr
    request_pair(Communicator<MessageChannelType> &requester) {
        auto me = *this;

        if (this->handler_pair_request(requester, 0) &&
            this->handler_pair_request(me, 0)) {
            this->channel =
                std::make_shared<MessageChannelType>(*this, requester);
            return this->channel;
        } else {
            return nullptr;
        }
    }

    // internal use only, called by this (may use other->handle, but that
    // means it's same as if that other had called it wrt to himself)
    bool handler_pair_request(Communicator<MessageChannelType> &requester,
                              int pairing_data) {
        // accept all pair requests
        return true;
    }

  public:
};

// If we have a MessageChannel, that implicitly means we are 'paired'
template <typename MessageChannelType>
class MessageSender : Communicator<MessageChannelType> {
  public:
    void send_message(MessageChannelType::MessageType &msg) {}
};
class MessageReceiver {
  public:
    virtual void message_receive_handler();
};

} // namespace communicator
