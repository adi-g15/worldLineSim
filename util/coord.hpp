#pragma once

#include <ostream>
#include <cstdint>
#include <array>

namespace util
{
    template<typename dimen_t = int32_t> // @assert - dimen_t must be integral type
    struct _coord{
        dimen_t mX, mY, mZ;

        inline void operator+=(const _coord<dimen_t>& to_add) {
            this->mX += to_add.mX;
            this->mY += to_add.mY;
            this->mZ += to_add.mZ;
        }

        inline void operator+=(const std::array<dimen_t,3>& to_add) {
            this->mX += to_add[0];
            this->mY += to_add[1];
            this->mZ += to_add[2];
        }

        inline bool operator==(const _coord<dimen_t>& second) const = default;

        inline bool operator<(const _coord<dimen_t>& second) const {
            if (this->mX < second.mX)  return true;
            else if (this->mY < second.mY) return true;
            else return this->mZ < second.mZ;
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
            os << coord.mY << ',';

            if (coord.mZ >= 0) {
                os << ' ';
                if (coord.mZ < 10)
                    os << ' ';
            }
            else if (coord.mZ > -10)
                os << ' ';
            os << coord.mZ;
            return os;
        }

        _coord() noexcept : _coord(dimen_t{}, dimen_t{}, dimen_t{}) {}
        _coord(dimen_t x, dimen_t y, dimen_t z) noexcept : mX(x), mY(y), mZ(z){}
    };
} // namespace util
