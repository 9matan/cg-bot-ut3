#include "Mimax_PCH.h"

#include <assert.h>
#include <random>
#include <time.h>

#include "mimax/common/Random.h"

namespace mimax {
namespace common {

    unsigned int GenerateRandomSeed()
    {
        return (unsigned int)time(nullptr);
    }

    unsigned int UpdateRandomSeed(unsigned int seed)
    {
        if (seed == 0)
            seed = GenerateRandomSeed();

        srand(seed);
        return seed;
    }

    unsigned int GetRandomUInt32(unsigned int const minVal, unsigned int const maxVal)
    {
        assert(minVal < maxVal);
        unsigned int const range = maxVal - minVal;
        return rand() % range + minVal;
    }

    size_t GetRandomSizeT(size_t const minVal, size_t const maxVal)
    {
        assert(minVal < maxVal);
        size_t const range = maxVal - minVal;
        return rand() % range + minVal;
    }
}
}