#pragma once

#include <thread>

#include "modules/communication/communicator.hpp"

// Communication channels
namespace interthread {
using communicator::Communicator;

class MessagePipe {
    Communicator ea, eb; // end a, end b
};

} // namespace interthread
