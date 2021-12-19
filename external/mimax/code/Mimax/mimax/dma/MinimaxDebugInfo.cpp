#include "Mimax_PCH.h"
#include "mimax/dma/MinimaxDebugInfo.h"

#if MIMAX_MINIMAX_DEBUG
namespace mimax {
namespace dma {

std::ostream& operator<<(std::ostream& o, SMinimaxDebugInfo const& debugInfo)
{
    size_t const mxDepth = debugInfo.m_maxDepth > SMinimaxDebugInfo::MAX_DEPTH
        ? SMinimaxDebugInfo::MAX_DEPTH
        : debugInfo.m_maxDepth + 1;

    o << "Evaluated nodes count: " << debugInfo.m_evaluatedNodesCnt << "\n";

    o << "Visited nodes count " << debugInfo.m_totalVisitedNodesCnt << ": \n";
    for (size_t depth = 1; depth <= mxDepth; ++depth)
    {
        o << depth << ": " << debugInfo.m_visitedNodesCnt[depth] << "\n";
    }

    o << "Pruned nodes count " << debugInfo.m_totalPrunedNodesCnt << ": \n";
    for (size_t depth = 1; depth <= mxDepth; ++depth)
    {
        o << depth << ": " << debugInfo.m_prunedNodesCnt[depth] << "\n";
    }

    return o;
}

} // dma
} // mimax
#endif // MIMAX_MINIMAX_DEBUG