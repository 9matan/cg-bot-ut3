#pragma once

//#define MIMAX_MCTS_DEBUG (1)

#if MIMAX_MCTS_DEBUG
#include <iostream>
#include <vector>

namespace mimax {
namespace dma {

struct SMctsDebugInfo
{
    std::vector<size_t> m_nodesCnt;
    std::vector<size_t> m_visitedNodesCnt;
    size_t m_totalNodesCnt;
    size_t m_totalVisitedNodesCnt;
    size_t m_simulationsCnt;

    SMctsDebugInfo()
    {
        Reset();
    }

    inline void ExpanseNode(size_t const nodeDepth, size_t const childrenCnt)
    {
        m_totalNodesCnt += childrenCnt;
        if (nodeDepth + 1 >= m_nodesCnt.size())
        {
            m_nodesCnt.resize(nodeDepth + 2, 0);
        }
        m_nodesCnt[nodeDepth + 1] += childrenCnt;
    }

    inline void VisitNode(size_t const nodeDepth)
    {
        ++m_totalVisitedNodesCnt;
        if (nodeDepth >= m_visitedNodesCnt.size())
        {
            m_visitedNodesCnt.resize(nodeDepth + 1, 0);
        }
        ++m_visitedNodesCnt[nodeDepth];
    }

    inline void Reset()
    {
        m_totalNodesCnt = 0;
        m_totalVisitedNodesCnt = 0;
        m_simulationsCnt = 0;
        m_nodesCnt.clear();
        m_nodesCnt.reserve(64);
        m_visitedNodesCnt.clear();
        m_visitedNodesCnt.reserve(64);
    }

    inline size_t GetMaxDepth() const { return m_visitedNodesCnt.size(); }
};

std::ostream& operator<<(std::ostream& o, SMctsDebugInfo const& debugInfo);

} // dma
} // mimax
#endif // MIMAX_MINIMAX_DEBUG