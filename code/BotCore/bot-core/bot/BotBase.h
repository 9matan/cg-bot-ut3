#pragma once

#include <chrono>

#include "mimax/common/Vec2.h"

#include "bot-core/ut3-game/GameState.h"

#include "bot-core/bot/IBot.h"

namespace ut3 {
namespace bot {

    class CBotBase : public IBot
    {
    public:
        CBotBase(char const* botName);

        SOutputData FirstUpdate(SInputData initData) override;
        SOutputData Update(SInputData turnData) override;
        void SetDebugIsEnabled(bool const enableDebug) override { m_isDebugEnabled = enableDebug; }
        void Reset() override;
        char const* GetBotName() const override { return m_botName.c_str(); }
        
        void SetState(game::SGameState const& gameState);
        inline void SetTimeout(std::chrono::milliseconds const timeout) { m_timeout = timeout; }

    protected:
        virtual SVec2 FindTurn(game::SGameState const& gameState) = 0;

    protected:
        std::string const m_botName;
        game::SGameState m_gameState;
        int m_myPlayer = -1;
        bool m_isDebugEnabled = false;
        std::chrono::milliseconds m_timeout;
    };

}
}