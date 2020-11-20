#pragma once

#include <cstdint>
#include <type_traits>

namespace util
{
	template<typename T>
	int num_digits(T number){
		static_assert( std::is_integral<T>::value == true );

		int count = 0;
		number = number > 0 ? number : -number;
		while (number != 0){
			number /= 10;
			++count;
		}

		return count;
	}

    template<typename T1, typename T2 = T1>
    bool min(T1 a, T2 b){
        static_assert( std::is_integral_v<T1> == true && std::is_integral_v<T2> == true );

        return a < b ? a : b;
    }

    template<typename T>
    bool abs(T a){
        static_assert( std::is_integral_v<T> == true );

        return a > 0 ? a : -a;
    }

} // namespace util
