#pragma once

#include <chrono>
#include <memory>

#include "mimax/common/Vec2.h"

#include "bot-core/ut3-game/GameState.h"

#include "bot-core/bot/IBot.h"

namespace ut3 {
namespace bot {

class IBotBehavior;

class CBot : public IBot
{
public:
    CBot(std::unique_ptr<IBotBehavior> botBehavior, char const* botName);
    ~CBot() override;

    SOutputData FirstUpdate(SInputData initData) override;
    SOutputData Update(SInputData turnData) override;
    void SetDebugIsEnabled(bool const enableDebug) override { m_isDebugEnabled = enableDebug; }
    void Reset() override;
    char const* GetBotName() const override { return m_botName.c_str(); }

    inline void SetTimeout(std::chrono::milliseconds const timeout) { m_timeout = timeout; }
    
    template<typename TBb>
    inline TBb* GetBb() { return static_cast<TBb*>(m_botBehavior.get()); }

private:
    void InitializeBB();
    SVec2 FindAndMakeTurn();

private:
    game::SGameState m_gameState;
    std::string const m_botName;
    std::unique_ptr<IBotBehavior> m_botBehavior;
    std::chrono::milliseconds m_timeout;
    bool m_isDebugEnabled;
};

} // bot
} // ut3