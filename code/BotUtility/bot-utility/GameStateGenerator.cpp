#include "BotUtility_PCH.h"
#include "bot-utility/GameStateGenerator.h"

#include "bot-core/ut3-game/Game.h"

namespace ut3::utility
{

CGameStateDepthGenerator::CGameStateDepthGenerator(const game::SGameState& rootState, const unsigned int depth)
    : m_rootState(rootState)
    , m_depth(depth)
{
}

std::vector<game::SGameState> CGameStateDepthGenerator::Generate()
{
    m_generatedStates.resize(0);
    Generate(m_rootState, 0);
    return m_generatedStates;
}


void CGameStateDepthGenerator::Generate(const game::SGameState gameState, unsigned int depth)
{
    if(depth == m_depth) return;

    game::Turns turns;
    game::CollectPossibleTurns(gameState, turns);

    for(const auto turn : turns)
    {
        auto& nextGameState = m_generatedStates.emplace_back(gameState);
        game::MakeTurn(nextGameState, turn[0], turn[1]);
        Generate(nextGameState, depth + 1);
    }
}

} // ut3::utility