#pragma once

#include <cassert>

namespace mimax {
namespace common {

    unsigned int GenerateRandomSeed();
    unsigned int UpdateRandomSeed(unsigned int seed = 0);
    // exclusive max
    unsigned int GetRandomUInt32(unsigned int const minVal, unsigned int const maxVal);
    size_t GetRandomSizeT(size_t const minVal, size_t const maxVal);

    template<typename TIter>
    inline TIter GetRandomItem(TIter first, TIter last)
    {
        size_t const rangeSize = static_cast<size_t>(last - first);
        return first + GetRandomUInt32(0, static_cast<unsigned int>(rangeSize));
    }

    template<typename TEnum>
    inline TEnum GetRandomEnumValue()
    {
        return static_cast<TEnum>(GetRandomUInt32(0, static_cast<unsigned int>(TEnum::Count)));
    }

    template<typename TIter>
    inline void RandomShuffle(TIter first, TIter last)
    {
        if (first == last) return;
        TIter current = last;
        --current;

        for (; current != first; --current)
        {
            std::swap(*current, *(first + GetRandomUInt32(0, (unsigned int)(current - first + 1))));
        }
    }

}
}