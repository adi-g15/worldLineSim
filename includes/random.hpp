#pragma once

#include <limits>

class Random{   //gives out non-zero values only
public:
    template<typename T>    //only integer types
    static T random(){
        static_assert(std::is_integral_v<T>);
        return random( 0, std::numeric_limits<T>::max() );
    }

    template<typename T>    //only integer types
    static T random(T max){
        static_assert(std::is_integral_v<T>);
        return random(T{0}, max);
    }

    template<typename T>    //only integer types
    static T random(T min, T max){ // including `min`, but excluding `max`
        static_assert(std::is_integral_v<T>);
        // @todo

        return T{};
    }

private:
    //static mt1667
};

typedef uint16_t id_type;
    // @todo - Ensure this is thread_safe, to concurrently create multiple entities in different worlds IN DIFFERENT VERSES
class _ID{  // no reuse of IDs, since it's enough
    public:
        const id_type _id;
        _ID(): _id(++(_ID::_curr_ID)){}
    private:
        static id_type _curr_ID;
        static std::mutex _id_mutex;    // to prevent data races with _ID::_curr_ID
};
