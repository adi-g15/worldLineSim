#pragma once

typedef uint64_t _timePoint;
typedef _timePoint _timeDiff;
namespace statics
{
    typedef int32_t dimen_t;
    // static constexpr std::pair< uint16_t, uint16_t > MAX_BOUNDS{ 1000, 1000 };    //applicable to ALL WORLDS CREATED
    // static constexpr std::pair< uint16_t, uint16_t > init_Max_Bound{ 40, 40 };    //applicable to A NEW WORLD
        // Currently using square matrix as parent, since it will be equally expanding later
    static constexpr uint16_t MAX_BOUND{ 1000 };    //applicable to ALL WORLDS CREATED
    static constexpr uint16_t init_Bound{ 40, };    //applicable to A NEW WORLD

    static constexpr float max_free_space = 1000.0f;    //so that space doesn't just get wasted
    static constexpr float min_free_space = 10.0f;    //if it falls below this, in a particular worlPlot, the expansion rate will be increased

    static constexpr _timePoint BIG_BANG_TIME = 0;    //can be time::now() later too
    static constexpr float UNIT_TIME = 1.0f;    //in terms of human world seconds

    static constexpr uint8_t init_expansion_speed{ 1 }; // @future - allow negative growths too

} // namespace statics
