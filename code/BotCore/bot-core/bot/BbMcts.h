#pragma once

#include "mimax/common/Vec2.h"

#include "mimax/dma/MctsBase.h"
#if MIMAX_MCTS_DEBUG
#include "mimax/dma/MctsDebugInfo.h"
#endif // MIMAX_MCTS_DEBUG


#include "bot-core/bot/BotBehaviorBase.h"
#include "bot-core/bot/BotBehaviorInitParams.h"

#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace bot {

template<size_t TCacheSize>
class CUT3MctsCachedMathHelper
{
public:
    static void Initialize();

    static inline float sqrt(float const val)
    {
        int const intVal = (int)(val * 100.0f + 0.00001f);
        if (intVal >= ms_sqrtCache.size()) return ::sqrt(val);
        return ms_sqrtCache[intVal];
    }

    static inline float log(int const val)
    {
        if (val >= ms_logCache.size()) return (float)::log(val);
        return ms_logCache[val];
    }

private:
    static std::array<float, TCacheSize> ms_sqrtCache;
    static std::array<float, TCacheSize> ms_logCache;
};

class CUT3MctsResolver
{
public:
    CUT3MctsResolver(size_t const maxDepth);

    inline void GetPossibleMoves(game::SGameState const& state, game::Turns& movesOut)
    {
        game::CollectPossibleTurns(state, movesOut);
    }

    inline void MakeMove(game::SGameState& state, SVec2 const move)
    {
        game::MakeTurn(state, move[0], move[1]);
    }
    
    float Playout(game::SGameState const& state);

private:
    size_t m_maxDepth;
};

using CUT3MctsMathHelper = CUT3MctsCachedMathHelper<30000>;
using CUT3Mcts = mimax::dma::CMctsBase<game::SGameState, SVec2, game::Turns, CUT3MctsResolver, CUT3MctsMathHelper>;

class CBbMcts : public CBotBehaviorBase
{
public:
    CBbMcts(float const explorationParam, size_t const maxDepth);

    void Initialize(game::SGameState const& gameState, SBotBehaviorInitParams const& initParams) override;
    SVec2 FindTurn(game::SGameState const& gameState) override;
    void Shutdown() override;

    //void OnMatchEnded() override;

#if MIMAX_MCTS_DEBUG
public:
    struct SDebugStats
    {
        float m_totalNodesCnt = 0.0f;
        float m_totalVisitedNodesCnt = 0.0f;
        float m_simulationsCnt = 0.0f;
    };

private:
    std::vector<SDebugStats> m_debugStatsHistory;
#endif // MIMAX_MCTS_DEBUG

private:
    std::vector<CUT3Mcts> m_mctsAlgos;
    SBotBehaviorInitParams m_params;
    float m_explorationParam;
    size_t m_maxDepth;
};

} // bot
} // ut3