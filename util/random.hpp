#pragma once

#include <limits>
#include <random>

namespace util{
    class Random{   //gives out non-zero values only
    public:
        template<typename T = uint32_t> // so that, if signed BUT positive integer type is required, it can be given (it required narrowing conversions)
        static T random(){
            static_assert( std::is_integral_v<T> );

            return random<T>(0, std::numeric_limits<uint32_t>::max() );
        }

        template<typename T = uint32_t>
        static T random(uint32_t max){
            static_assert( std::is_integral_v<T> );

            return random<T>(0, max);
        }

        template<typename T = uint32_t>
        static T random(uint32_t min, uint32_t max){ // [`min`, `max`)     ie. max excluded
            static_assert( std::is_integral_v<T> );

            uint32_t num = std::random_device{}();
            // @future - Later use this generator too

            // @fix_me - Fix later-> won't work when (num-min) < 0
            return static_cast<T>((((num-min)%max) + num ));
        }

    private:
        // static std::mt19937 generator(std::random_device{}());
        static std::random_device device;

    };
}