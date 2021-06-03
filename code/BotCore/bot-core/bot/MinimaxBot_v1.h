#pragma once

#include "mimax/common/Vec2.h"

#include "bot-core/bot/IBot.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3
{
    class CMinimaxBot_v1 : public IBot
    {
    public:
        static SVec2 FindTurn(game::SGameState const& gameState, int const myPlayer, bool const debugEnabled = false);
    public:
        SOutputData FirstUpdate(SInputData initData) override;
        SOutputData Update(SInputData turnData) override;
        void SetDebugIsEnabled(bool const enableDebug) override { m_isDebugEnabled = enableDebug; }
        char const* GetBotName() const override { return "CMinimaxBot_v1"; }
    private:
        int m_myPlayer = -1;
        game::SGameState m_gameState;
        bool m_isDebugEnabled = false;
    };
}