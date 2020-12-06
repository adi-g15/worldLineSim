#pragma once

#include <ostream>
#include <cstdint>

namespace util
{
    template<typename dimen_t = int32_t> // @assert - dimen_t must be integral type
    struct _coord{
        dimen_t mX, mY;

        inline bool operator==(const _coord<dimen_t>& second) const = default;

        inline bool operator<(const _coord<dimen_t>& second) const {
            if (this->mX < second.mX)  return true;
            else
                return this->mY < second.mY;
        }

        inline bool operator>(const _coord<dimen_t>& second) const {
            return !this->operator==(second) && !this->operator<(second);
        }

        friend std::ostream& operator<<(std::ostream& os, const _coord<dimen_t>& coord){
            if(coord.mX >= 0){
                os << ' ';
                if(coord.mX < 10)
                    os << ' ';
            }else if(coord.mX > -10)
                os << ' ';
            os << coord.mX <<',';
            if(coord.mY >= 0){
                os << ' ';
                if(coord.mY < 10)
                    os << ' ';
            }else if(coord.mY > -10)
                os << ' ';

            os << coord.mY;
            return os;
        }

        _coord(dimen_t x, dimen_t y): mX(x), mY(y){}
    };
} // namespace util
