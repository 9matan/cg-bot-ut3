#pragma once

#include <cmath>

namespace mimax {
namespace common {

class CMathHelper
{
public:
    static inline float log(float const val) { return ::log(val); }
    static inline float sqrt(float const val) { return ::sqrt(val); }
};

} // common
} // mimax