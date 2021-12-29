#include "Mimax_PCH.h"
#include "mimax/dma/MctsDebugInfo.h"

#if MIMAX_MCTS_DEBUG
namespace mimax {
namespace dma {

std::ostream& operator<<(std::ostream& o, SMctsDebugInfo const& debugInfo)
{
    o << "Max depth: " << debugInfo.GetMaxDepth() << "\n";
    o << "Simulations count: " << debugInfo.m_simulationsCnt << "\n";

    o << "Nodes count " << debugInfo.m_totalNodesCnt << ": \n";
    for (size_t depth = 1; depth <= debugInfo.m_nodesCnt.size(); ++depth)
    {
        o << depth << ": " << debugInfo.m_nodesCnt[depth] << "\n";
    }

    o << "Visited nodes count " << debugInfo.m_totalVisitedNodesCnt << ": \n";
    for (size_t depth = 1; depth <= debugInfo.m_visitedNodesCnt.size(); ++depth)
    {
        o << depth << ": " << debugInfo.m_visitedNodesCnt[depth] << "\n";
    }

    return o;
}

} // dma
} // mimax
#endif // MIMAX_MINIMAX_DEBUG