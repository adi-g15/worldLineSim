#pragma once

#include <limits>

class Random{   //gives out non-zero values only
public:
    template<typename T>    //only integer types
    static T random(){
        return random( 0, std::numeric_limits<T>::max() );
    }

    template<typename T>    //only integer types
    static T random(T max){
        return random(T{0}, max);
    }

    template<typename T>    //only integer types
    static T random(T min, T max){ // including `min`, but excluding `max`
        // @todo

        return T{};
    }

private:
    //static mt1667
};
