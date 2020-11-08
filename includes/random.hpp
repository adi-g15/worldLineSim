#pragma once

#include <limits>
#include <random>

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
    static std::mt19937 generator(std::random_device{});

};
