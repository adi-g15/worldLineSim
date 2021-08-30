#pragma once

#include <chrono>

using namespace std::chrono_literals;

using _timePoint = uint64_t;
using _timeDiff = _timePoint;
namespace statics
{
    static constexpr const char* DATABASE_DIRNAME = "db";   // Relative to project root
    
    using dimen_t = int32_t;
    // static constexpr std::pair< uint16_t, uint16_t > MAX_BOUNDS{ 1000, 1000 };    //applicable to ALL WORLDS CREATED
    // static constexpr std::pair< uint16_t, uint16_t > init_Max_Bound{ 40, 40 };    //applicable to A NEW WORLD
        // Currently using square matrix as parent, since it will be equally expanding later
    static constexpr uint16_t MAX_BOUND{ 1000 };    //applicable to ALL WORLDS CREATED
    static constexpr uint16_t init_Bound{ 40, };    //applicable to A NEW WORLD

    static constexpr float max_free_space = 1000.0f;    //so that space doesn't just get wasted
    static constexpr float min_free_space = 10.0f;    //if it falls below this, in a particular worlPlot, the expansion rate will be increased

    static constexpr _timePoint BIG_BANG_TIME = 0;    //can be time::now() later too
    static constexpr std::chrono::seconds UNIT_TIME = 1s;

    static constexpr uint8_t init_expansion_speed{ 1 }; // @future - allow negative growths too

} // namespace statics
