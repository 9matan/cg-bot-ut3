#include "MimaxCommon_PCH.h"

#include <assert.h>
#include <random>
#include <time.h>

namespace mimax {
namespace common {

unsigned int UpdateRandomSeed()
{
    unsigned int const seed = static_cast<unsigned int>(time(nullptr));
    srand(seed);
    return seed;
}

unsigned int GetRandomUInt(unsigned int const minVal, unsigned int const maxVal)
{
    assert(minVal < maxVal);
    unsigned int const range = maxVal - minVal;
    return rand() % range + minVal;
}

}
}