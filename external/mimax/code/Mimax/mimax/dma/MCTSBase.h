#include <algorithm>
#include <random>
#include <vector>

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
class CMCTSBase
{
public:
    CMCTSBase(TState const& rootSate, TResolver const& resolver, unsigned long long const randomSeed, float const explorationParam = 1.4142f)
        : m_resolver(resolver)
        , m_randomEngine(randomSeed)
        , m_explorationParam(explorationParam)
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
        SNode* m_parent;
        unsigned int m_simulationsCount;
        float m_score;
        float m_uctScore;
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

        return !m_movesBuffer.empty();
    }

    void MakeIteration()
    {
        SNode* curNode = SelectChildNode(&m_root);
        while (curNode->IsVisited())
        {
            if (!curNode->HasChildren() && !Expanse(curNode))
            {
                break;
            }
            curNode = SelectChildNode(curNode);
        }

        float const score = curNode->IsVisited()
            ? curNode->m_score / (float)curNode->m_simulationsCount
            : VisitNode(curNode);

        while (curNode != nullptr)
        {
            UpdateStatistics(curNode, score);
            curNode = curNode->m_parent;
        }
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
        return  (float)node->m_score / (float)node->m_simulationsCount
            + m_explorationParam * (float)sqrt(log(parentSimCount) / node->m_simulationsCount);
    }
};

} // dma
} // mimax