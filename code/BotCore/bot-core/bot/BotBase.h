#pragma once

#include "mimax/common/Vec2.h"

#include "bot-core/ut3-game/GameState.h"

#include "bot-core/bot/IBot.h"

namespace ut3 {
namespace bot {

    class CBotBase : public IBot
    {
    public:
        SOutputData FirstUpdate(SInputData initData) override;
        SOutputData Update(SInputData turnData) override;
        void SetDebugIsEnabled(bool const enableDebug) override { m_isDebugEnabled = enableDebug; }
        void Reset() override;

    protected:
        virtual SVec2 FindTurn(game::SGameState const& gameState) = 0;

    protected:
        game::SGameState m_gameState;
        int m_myPlayer = -1;
        bool m_isDebugEnabled = false;
    };

}
}