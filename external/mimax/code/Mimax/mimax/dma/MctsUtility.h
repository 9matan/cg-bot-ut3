#pragma once

#include <cassert>
#include <vector>

#include "mimax/dma/MctsNodeEvalResult.h"

namespace mimax {
namespace dma {
namespace mcts_utility {

template<typename TMove>
void CombineEvalResults(SMctsNodeEvalResult<TMove>& result, SMctsNodeEvalResult<TMove> const& resultToCombine)
{
    auto& resultChildrenInfo = result.m_childrenInfo;
    for (auto const& childInfo : resultToCombine.m_childrenInfo)
    {
        auto iter = find_if(resultChildrenInfo.begin(), resultChildrenInfo.end(),
            [move = childInfo.m_move](auto const& info)
        {
            return move == info.m_move;
        });

        if (iter != resultChildrenInfo.end())
        {
            iter->m_simulationsCount += childInfo.m_simulationsCount;
        }
        else
        {
            resultChildrenInfo.push_back(childInfo);
        }
    }
}

template<typename TMove>
TMove GetBestMove(SMctsNodeEvalResult<TMove> const& evalResult)
{
    assert(!evalResult.m_childrenInfo.empty());

    return std::max_element(evalResult.m_childrenInfo.begin(), evalResult.m_childrenInfo.end(),
        [](auto const& lhs, auto const& rhs)
        {
            return lhs.m_simulationsCount < rhs.m_simulationsCount;
        })->m_move;
}

} // mcts_utility
} // dma
} // mimax