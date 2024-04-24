#pragma once

#include "mimax/common/Vec2.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace bot {

struct SBotBehaviorInitParams;

class IBotBehavior
{
public:
    virtual ~IBotBehavior() = default;

    virtual void Initialize(game::SGameState const& gameState, SBotBehaviorInitParams const& initParams) = 0;
    virtual SVec2 FindTurn(game::SGameState const& gameState) = 0;
    virtual void Shutdown() = 0;
};

} // bot
} // ut3