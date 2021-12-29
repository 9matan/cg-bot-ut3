#include <algorithm>
#include <random>
#include <vector>

#include "mimax/dma/MctsDebugInfo.h"
#include "mimax/dma/MctsNodeEvalResult.h"

namespace mimax {
namespace dma {

/*
TMovesContainer
    iter begin()
    iter end()
    bool empty()
    sie_t size()
*/

/*
TResolver
    void GetPossibleMoves(TState const&, TMovesContainer&)
    void MakeMove(TState&, TMove const&)
    float Playout(TState const&)
*/

template<typename TState, typename TMove, typename TMovesContainer, typename TResolver>
class CMctsBase
{
public:
    CMctsBase(TState const& rootSate, TResolver const& resolver, unsigned long long const randomSeed, float const explorationParam = 1.4142f)
        : m_resolver(resolver)
        , m_randomEngine(randomSeed)
        , m_explorationParam(explorationParam)
#if MIMAX_MCTS_DEBUG
        , m_currentDepth(0)
#endif // MIMAX_MCTS_DEBUG
    {
        m_root = SNode();
        m_root.m_state = rootSate;
        bool const successful = Expanse(&m_root);
        assert(successful);
    }

    void Evaluate()
    {
        MakeIteration();
    }

    TMove GetCurrentResult() const
    {
        return std::max_element(m_root.m_children.begin(), m_root.m_children.end(),
            [](SNode const& lhs, SNode const& rhs)
            {
                return lhs.m_simulationsCount < rhs.m_simulationsCount;
            })->m_move;
    }

    SMctsNodeEvalResult<TMove> GetRootEvalResult() const
    {
        SMctsNodeEvalResult<TMove> evalResult;

        evalResult.m_childrenInfo.resize(m_root.m_children.size());
        for (size_t i = 0; i < m_root.m_children.size(); ++i)
        {
            evalResult.m_childrenInfo[i].m_move = m_root.m_children[i].m_move;
            evalResult.m_childrenInfo[i].m_simulationsCount = m_root.m_children[i].m_simulationsCount;
        }

        return std::move(evalResult);
    }

#if MIMAX_MCTS_DEBUG
    inline SMctsDebugInfo const& GetDebugInfo() const { return m_debugInfo; }
#endif // MIMAX_MCTS_DEBUG

private:
    struct SNode
    {
        SNode()
            : m_simulationsCount(0)
            , m_score(0)
            , m_uctScore(0.0f)
            , m_parent(nullptr)
            , m_unvisitedChildrenCount(0)
        {}

        inline bool IsVisited() const { return m_simulationsCount > 0; }
        inline bool HasChildren() const { return !m_children.empty(); }
        inline bool HasUnvisitedChildren() const { return m_unvisitedChildrenCount > 0; }

        std::vector<SNode> m_children;
        unsigned int m_simulationsCount;
        float m_score;
        float m_uctScore;
        SNode* m_parent;
        TState m_state;
        TMove m_move;
        unsigned short m_unvisitedChildrenCount;
    };

private:
    std::mt19937_64 m_randomEngine;
    TResolver m_resolver;
    SNode m_root;
    float m_explorationParam;
    TMovesContainer m_movesBuffer;
#if MIMAX_MCTS_DEBUG
    SMctsDebugInfo m_debugInfo;
    size_t m_currentDepth;
#endif // MIMAX_MCTS_DEBUG

private:
    bool Expanse(SNode* node)
    {
        m_resolver.GetPossibleMoves(node->m_state, m_movesBuffer);
        std::shuffle(m_movesBuffer.begin(), m_movesBuffer.end(), m_randomEngine);

        node->m_children.resize(m_movesBuffer.size());
        auto curChild = node->m_children.begin();
        for (auto const move : m_movesBuffer)
        {
            curChild->m_parent = node;
            curChild->m_move = move;

            ++curChild;
        }
        node->m_unvisitedChildrenCount = (unsigned short)node->m_children.size();

#if MIMAX_MCTS_DEBUG
        m_debugInfo.ExpanseNode(m_currentDepth, node->m_children.size());
#endif // MIMAX_MCTS_DEBUG

        return !m_movesBuffer.empty();
    }

    void MakeIteration()
    {
        SNode* curNode = SelectChildNode(&m_root);
#if MIMAX_MCTS_DEBUG
        m_currentDepth = 1;
#endif // MIMAX_MCTS_DEBUG
        while (curNode->IsVisited())
        {
            if (!curNode->HasChildren() && !Expanse(curNode))
            {
                break;
            }
            curNode = SelectChildNode(curNode);
#if MIMAX_MCTS_DEBUG
            ++m_currentDepth;
#endif // MIMAX_MCTS_DEBUG
        }

        float score = curNode->IsVisited()
            ? curNode->m_score / (float)curNode->m_simulationsCount
            : VisitNode(curNode);

        while (curNode != nullptr)
        {
            UpdateStatistics(curNode, score);
            score = 1.0f - score;
            curNode = curNode->m_parent;
#if MIMAX_MCTS_DEBUG
            --m_currentDepth;
#endif // MIMAX_MCTS_DEBUG
        }
#if MIMAX_MCTS_DEBUG
        ++m_debugInfo.m_simulationsCnt;
#endif // MIMAX_MCTS_DEBUG
    }

    inline SNode* SelectChildNode(SNode* node)
    {
        return node->HasUnvisitedChildren()
            ? GetFirstUnvisitedChild(node)
            : GetBestNodeByUCT(node);
    }

    inline float VisitNode(SNode* node)
    {
        auto parent = node->m_parent;
        --parent->m_unvisitedChildrenCount;
        node->m_state = parent->m_state;
        m_resolver.MakeMove(node->m_state, node->m_move);
#if MIMAX_MCTS_DEBUG
        m_debugInfo.VisitNode(m_currentDepth);
#endif // MIMAX_MCTS_DEBUG
        return m_resolver.Playout(node->m_state);
    }

    inline void UpdateStatistics(SNode* node, float const score)
    {
        ++node->m_simulationsCount;
        node->m_score += score;
    }

    inline SNode* GetFirstUnvisitedChild(SNode* node)
    {
        auto iter = std::find_if(node->m_children.begin(), node->m_children.end(),
            [](SNode const& child)
            {
                return !child.IsVisited();
            });
        assert(iter != node->m_children.end());
        return &(*iter);
    }

    inline SNode* GetBestNodeByUCT(SNode* node)
    {
        for (auto& child : node->m_children)
        {
            child.m_uctScore = CalculateUCTScore(&child);
        }
        auto iter = std::max_element(node->m_children.begin(), node->m_children.end(),
            [](SNode const& lhs, SNode const& rhs)
            {
                return lhs.m_uctScore < rhs.m_uctScore;
            });
        return &(*iter);
    }

    inline float CalculateUCTScore(SNode const* node)
    {
        auto const parentSimCount = node->m_parent->m_simulationsCount;
        return  1.0f - (float)node->m_score / (float)node->m_simulationsCount
            + m_explorationParam * (float)sqrt(log(parentSimCount) / node->m_simulationsCount);
    }
};

} // dma
} // mimax