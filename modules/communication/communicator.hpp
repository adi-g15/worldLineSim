#pragma once

#include "id_creator.hpp"

namespace communicator {
class Communicator {
    id_type my_id;

  public:
    Communicator(id_type id) : my_id(id) {}
};
} // namespace communicator
