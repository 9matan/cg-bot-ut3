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
        int m_evaluatedNodesCnt;
        int m_visitedNodesCnt;

        inline void Reset()
        {
            m_evaluatedNodesCnt = 0;
            m_visitedNodesCnt = 0;
        }
    };
#endif // MIMAX_MINIMAX_DEBUG

public:
    CMinimaxAlgorithmBase(size_t const maxDepth, TResolver const& resolver)
        : m_maxDepth(maxDepth)
        , m_resolver(resolver)
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
        , m_minValue(-std::numeric_limits<float>::max())
        , m_maxValue(std::numeric_limits<float>::max())
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    {}

#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    CMinimaxAlgorithmBase(size_t const maxDepth, TResolver const& resolver, float const minValue, float const maxValue)
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
    size_t m_maxDepth;
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    float m_minValue;
    float m_maxValue;
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
#if MIMAX_MINIMAX_DEBUG
    SDebugInfo m_debugInfo;
#endif // MIMAX_MINIMAX_DEBUG

private:
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
    STraversalResult VisitState(TState const& state, size_t const depth, float alpha, float beta)
#else
    STraversalResult VisitState(TState const& state, size_t const depth)
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

        for(auto const move: moves)
        {
            TState childState = state;
            m_resolver.MakeMove(childState, move);
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            auto const childResult = VisitState(childState, depth + 1, -beta, -alpha);
#else
            auto const childResult = VisitState(childState, depth + 1);
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            if(-childResult.m_score > result.m_score)
            {
                result.m_move = move;
                result.m_score = -childResult.m_score;
#if MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
                alpha = (result.m_score > alpha) ? result.m_score : alpha;
                if (alpha >= beta) break;
#endif // MIMAX_MINIMAX_ENABLE_ALPHA_BETA_PRUNING
            }
        }

        return result;
    }
};

}
}