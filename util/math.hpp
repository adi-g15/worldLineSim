#pragma once

#include <cstdint>

namespace util
{
    bool min(uint32_t a, uint32_t b){
        return a < b ? a : b;
    }
} // namespace util
