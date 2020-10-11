#pragma once

#include <utility>
#include <cstdint>

struct _coord{
    private: static constexpr uint32_t _NULL_COORDINATE_ = static_cast<uint32_t>(-1);
    public:
    uint32_t mX, mY;

    inline bool operator==(const _coord& c) const{ return this->mX == c.mX && this->mY == c.mY; }
    inline void reset(){ this->mX = _NULL_COORDINATE_; this->mY = _NULL_COORDINATE_; }
    inline bool isNull(){ return this->mX == _NULL_COORDINATE_ && this->mY == _NULL_COORDINATE_; }
};

typedef uint64_t _timePoint;

struct _box{
    bool isEmpty = true;
};

namespace statics
{
    static constexpr std::pair< uint16_t, uint16_t > MAX_BOUNDS{ 1000, 1000 };    //applicable to ALL WORLDS CREATED
    static constexpr std::pair< uint16_t, uint16_t > init_Max_Bound{ 40, 40 };    //applicable to A NEW WORLD

    static constexpr _timePoint BIG_BANG_TIME = 0;    //can be time::now() later too
    static constexpr float UNIT_TIME = 1.0f;    //in terms of human world seconds

} // namespace statics

