#pragma once

#include "bot-core/InputData.h"
#include "bot-core/OutputData.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3
{
    class CMinimaxBot
    {
    public:
        CMinimaxBot(SInputData initData);

        SOutputData FirstUpdate(SInputData initData);
        SOutputData Update(SInputData turnData);
    private:
        int m_myPlayer;
        game::SGameState m_gameState;
    };
}