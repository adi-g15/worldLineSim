#pragma once

#include <mutex>

#include "util/random.hpp"

typedef uint16_t id_type;
    // @todo - Ensure this is thread_safe, to concurrently create multiple entities in different worlds IN DIFFERENT VERSES
class _ID{  // no reuse of IDs, since it's enough
    public:
        const id_type _id;
        _ID(): _id(++(_ID::_curr_ID)){}
        id_type getId(){ return this->_id; }
    private:
        static id_type _curr_ID;
        static std::mutex _id_mutex;    // to prevent data races with _ID::_curr_ID
};
