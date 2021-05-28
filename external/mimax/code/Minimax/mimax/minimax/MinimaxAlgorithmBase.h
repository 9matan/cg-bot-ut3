#pragma once

#include <limits>

// Define MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING (1) to enabled pruning
#define MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING (1)

namespace mimax {
namespace minimax {

// TResolver
// float EvaluateState(TState const&);
// void GetPossibleMoves(TMovesContainer&, TState const&);
// void MakeMove(TState&, TMove);

// TMovesContainer
// bool empty();
// begin(), end()

template<typename TState, typename TMove, typename TMovesContainer, typename TResolver>
class CMinimaxAlgorithmBase
{
#if MIMAX_MINIMAX_DEBUG
public:
    struct SDebugInfo
    {
        unsigned int m_evaluatedNodesCnt;
        unsigned int m_visitedNodesCnt;
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        unsigned int m_prunedNodesCnt[64];
        unsigned int m_maxDepth;
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING

#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        inline void AddPrunedNodes(unsigned int const nodeCnt, unsigned int const depth)
        {
            if (depth < 64)
                m_prunedNodesCnt[depth] += nodeCnt;
        }
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING

        inline void Reset()
        {
            m_evaluatedNodesCnt = 0;
            m_visitedNodesCnt = 0;
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            m_maxDepth = 0;
            memset(m_prunedNodesCnt, 0, sizeof(m_prunedNodesCnt));
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        }

        inline void Print() const
        {
            std::cerr << "Visited nodes count: " << m_visitedNodesCnt << "\n";
            std::cerr << "Evaluated nodes count: " << m_evaluatedNodesCnt << "\n";
            std::cerr << "Pruned nodes count:\n";
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            for (unsigned int depth = 1; depth < std::min(64U, m_maxDepth + 1); ++depth)
            {
                std::cerr << depth << ": " << m_prunedNodesCnt[depth] << "\n";
            }
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        }
    };
#endif // MIMAX_MINIMAX_DEBUG

public:
    CMinimaxAlgorithmBase(unsigned int const maxDepth, TResolver const& resolver)
        : m_maxDepth(maxDepth)
        , m_resolver(resolver)
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        , m_minValue(-std::numeric_limits<float>::max())
        , m_maxValue(std::numeric_limits<float>::max())
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    {}

#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    CMinimaxAlgorithmBase(unsigned int const maxDepth, TResolver const& resolver, float const minValue, float const maxValue)
        : m_maxDepth(maxDepth)
        , m_resolver(resolver)
        , m_minValue(minValue)
        , m_maxValue(maxValue)
    {}
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING

    inline TMove Solve(TState const& state)
    {
#if MIMAX_MINIMAX_DEBUG
        m_debugInfo.Reset();
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        m_debugInfo.m_maxDepth = m_maxDepth;
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
#endif // MIMAX_MINIMAX_DEBUG
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        return VisitState(state, 0, m_minValue, m_maxValue).m_move;
#else
        return VisitState(state, 0).m_move;
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    }

#if MIMAX_MINIMAX_DEBUG
    inline SDebugInfo const& GetDebugInfo() const { return m_debugInfo; }
#endif // MIMAX_MINIMAX_DEBUG

private:
    struct STraversalResult
    {
        float m_score = 0.0f;
        TMove m_move; 
    };

private:
    TResolver m_resolver;
    unsigned int m_maxDepth;
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    float m_minValue;
    float m_maxValue;
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
#if MIMAX_MINIMAX_DEBUG
    SDebugInfo m_debugInfo;
#endif // MIMAX_MINIMAX_DEBUG

private:
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    STraversalResult VisitState(TState const& state, unsigned int const depth, float alpha, float beta)
#else
    STraversalResult VisitState(TState const& state, unsigned int const depth)
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    {
#if MIMAX_MINIMAX_DEBUG
        ++m_debugInfo.m_visitedNodesCnt;
#endif // MIMAX_MINIMAX_DEBUG
        TMovesContainer moves;
        if(depth != m_maxDepth)
        {
            m_resolver.GetPossibleMoves(moves, state);
        }
        STraversalResult result;
        if(moves.empty())
        {
#if MIMAX_MINIMAX_DEBUG
            ++m_debugInfo.m_evaluatedNodesCnt;
#endif // MIMAX_MINIMAX_DEBUG
            int const colorMultiplier = (depth & 1) == 0 ? 1 : -1;
            result.m_score = m_resolver.EvaluateState(state) * colorMultiplier;
            return result; 
        }

        result.m_score = -std::numeric_limits<float>::max();

#if MIMAX_MINIMAX_DEBUG
        unsigned int visitedChildrenCount = 0;
#endif // MIMAX_MINIMAX_DEBUG
        for(auto const move: moves)
        {
            TState childState = state;
            m_resolver.MakeMove(childState, move);
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            auto const childResult = VisitState(childState, depth + 1, -beta, -alpha);
#else
            auto const childResult = VisitState(childState, depth + 1);
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
#if MIMAX_MINIMAX_DEBUG
            ++visitedChildrenCount;
#endif // MIMAX_MINIMAX_DEBUG
            if(-childResult.m_score > result.m_score)
            {
                result.m_move = move;
                result.m_score = -childResult.m_score;
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
                alpha = (result.m_score > alpha) ? result.m_score : alpha;
                if (alpha >= (beta - 0.000001f))
                {
#if MIMAX_MINIMAX_DEBUG
                    m_debugInfo.AddPrunedNodes((unsigned int)moves.size() - visitedChildrenCount, depth + 1);
#endif // MIMAX_MINIMAX_DEBUG
                    break;
                }
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            }
        }

        return result;
    }
};

}
}