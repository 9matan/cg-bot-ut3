#include <algorithm>
#include <random>
#include <vector>

#include "mimax/common/MathHelper.h"
#include "mimax/common/ObjectPool.h"
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

template<
    typename TState,
    typename TMove,
    typename TMovesContainer,
    typename TResolver,
    typename TMathHelper = mimax::common::CMathHelper>
class CMctsBase
{
public:
    struct SConfig
    {
        size_t m_randomSeed;
        size_t m_nodesPoolSize = 65000;
        float m_explorationParam = 1.4142f;
    };

public:
    CMctsBase(TResolver const& resolver, SConfig const& config)
        : m_resolver(resolver)
        , m_randomEngine(config.m_randomSeed)
        , m_config(config)
#if MIMAX_MCTS_DEBUG
        , m_currentDepth(0)
#endif // MIMAX_MCTS_DEBUG
    {
    }

    void Reset(TState const& state)
    {
#if MIMAX_MCTS_DEBUG
        m_debugInfo.Reset();
        m_currentDepth = 0;
#endif // MIMAX_MCTS_DEBUG
        m_nodesPool.ResetPool(m_config.m_nodesPoolSize);
        m_root = AllocateNode();
        m_root->m_state = state;
        bool const successful = Expanse(m_root);
        assert(successful);
    }

    void Evaluate()
    {
        MakeIteration();
    }

    TMove GetCurrentResult() const
    {
        SNode* childNode = m_root->m_firstChild;
        SNode* bestNode = m_root->m_firstChild;

        while (childNode != nullptr)
        {
            if (childNode->m_simulationsCount > bestNode->m_simulationsCount)
            {
                bestNode = childNode;
            }

            childNode = childNode->m_nextNode;
        }

        return bestNode->m_move;
    }

    SMctsNodeEvalResult<TMove> GetRootEvalResult() const
    {
        SMctsNodeEvalResult<TMove> evalResult;

        SNode* childNode = m_root->m_firstChild;
        while(childNode != nullptr)
        {
            auto& info = evalResult.m_childrenInfo.emplace_back();
            info.m_move = childNode->m_move;
            info.m_simulationsCount = childNode->m_simulationsCount;
            childNode = childNode->m_nextNode;
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
            , m_parent(nullptr)
            , m_nextNode(nullptr)
            , m_firstChild(nullptr)
            , m_unvisitedChildrenCount(0)
        {}

        inline bool IsVisited() const { return m_simulationsCount > 0; }
        inline bool HasChildren() const { return m_firstChild != nullptr; }
        inline bool HasUnvisitedChildren() const { return m_unvisitedChildrenCount > 0; }

        unsigned int m_simulationsCount;
        float m_score;
        SNode* m_parent;
        SNode* m_nextNode;
        SNode* m_firstChild;
        TState m_state;
        TMove m_move;
        unsigned short m_unvisitedChildrenCount;
    };

private:
    std::mt19937_64 m_randomEngine;
    TResolver m_resolver;
    SNode* m_root;
    SConfig m_config;
    TMovesContainer m_movesBuffer;
    mimax::common::CObjectPool<SNode> m_nodesPool;
#if MIMAX_MCTS_DEBUG
    SMctsDebugInfo m_debugInfo;
    size_t m_currentDepth;
#endif // MIMAX_MCTS_DEBUG

private:
    inline SNode* AllocateNode()
    {
        if (m_nodesPool.IsEmpty()) return nullptr;
        return m_nodesPool.AllocateObject();
    }

    bool Expanse(SNode* node)
    {
        m_resolver.GetPossibleMoves(node->m_state, m_movesBuffer);
        std::shuffle(m_movesBuffer.begin(), m_movesBuffer.end(), m_randomEngine);

        SNode** curChildIter = &(node->m_firstChild);
        for (auto const move : m_movesBuffer)
        {
            SNode* curChild = AllocateNode();
            if (curChild == nullptr) break;

            curChild->m_parent = node;
            curChild->m_move = move;

            *curChildIter = curChild;
            curChildIter = &(curChild->m_nextNode);
            
            ++node->m_unvisitedChildrenCount;
        }

#if MIMAX_MCTS_DEBUG
        m_debugInfo.ExpanseNode(m_currentDepth, node->m_unvisitedChildrenCount);
#endif // MIMAX_MCTS_DEBUG

        return node->m_unvisitedChildrenCount > 0;
    }

    void MakeIteration()
    {
        SNode* curNode = SelectChildNode(m_root);
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
        auto childNode = node->m_firstChild;
        while (childNode != nullptr)
        {
            if (!childNode->IsVisited())
            {
                return childNode;
            }
            childNode = childNode->m_nextNode;
        }
        assert(false);
        return nullptr;
    }

    inline SNode* GetBestNodeByUCT(SNode* node)
    {
        assert(node->HasChildren());

        SNode* childNode = node->m_firstChild;
        SNode* bestNode = node->m_firstChild;
        float bestUCTScore = 0.0f;

        while (childNode != nullptr)
        {
            float const childUCTScore = CalculateUCTScore(childNode);
            if (childUCTScore > bestUCTScore)
            {
                bestNode = childNode;
                bestUCTScore = childUCTScore;
            }
            childNode = childNode->m_nextNode;
        }

        return bestNode;
    }

    inline float CalculateUCTScore(SNode const* node)
    {
        auto const parentSimCount = node->m_parent->m_simulationsCount;
        return  1.0f - (float)node->m_score / (float)node->m_simulationsCount
            + m_config.m_explorationParam * TMathHelper::sqrt(TMathHelper::log(parentSimCount) / node->m_simulationsCount);
    }
};

} // dma
} // mimax