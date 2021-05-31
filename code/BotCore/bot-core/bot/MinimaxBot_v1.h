#pragma once

#include "mimax/common/Vec2.h"

#include "bot-core/InputData.h"
#include "bot-core/OutputData.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3
{
    class CMinimaxBot_v1
    {
    public:
        static SVec2 FindTurn(game::SGameState const& gameState, int const myPlayer);
    public:
        CMinimaxBot_v1(SInputData initData);

        SOutputData FirstUpdate(SInputData initData);
        SOutputData Update(SInputData turnData);
    private:
        int m_myPlayer;
        game::SGameState m_gameState;
    };
}