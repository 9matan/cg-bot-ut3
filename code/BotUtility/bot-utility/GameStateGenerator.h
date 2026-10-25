#pragma once

#include <vector>

#include "bot-core/ut3-game/GameState.h"

namespace ut3::utility {

class CGameStateDepthGenerator
{
    ;

public:
    CGameStateDepthGenerator(const game::SGameState& rootState, const unsigned int depth);

    std::vector<game::SGameState> Generate();

private:
    std::vector<game::SGameState> m_generatedStates;
    game::SGameState m_rootState;
    unsigned int m_depth;

private:
    void Generate(const game::SGameState gameState, unsigned int depth);
};

} // ut3::utility