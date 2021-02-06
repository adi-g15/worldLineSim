#pragma once

#include <atomic>

#include "util/random.hpp"

typedef unsigned int id_type;
    // @todo - Ensure this is thread_safe, to concurrently create multiple entities in different worlds IN DIFFERENT VERSES
class _ID{  // no reuse of IDs, since it's enough
    public:
        const id_type _id;
        _ID(): _id(++(_ID::_curr_ID)){}
        id_type getId() const noexcept { return this->_id; }
    private:
        static inline std::atomic<id_type> _curr_ID{111110};
};
