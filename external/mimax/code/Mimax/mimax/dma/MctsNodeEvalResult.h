#pragma once

#include <vector>

namespace mimax {
namespace dma {

template<typename TMove>
struct SMctsNodeEvalResult
{
    struct SChildInfo
    {
        TMove m_move;
        unsigned int m_simulationsCount;

        bool operator==(SChildInfo const& other) const
        {
            return m_move == other.m_move
                && m_simulationsCount == other.m_simulationsCount;
        }
    };

    std::vector<SChildInfo> m_childrenInfo;
};

} // dma
} // mimax