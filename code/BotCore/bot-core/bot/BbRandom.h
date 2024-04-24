#pragma once

#include "bot-core/bot/BotBehaviorBase.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace bot {

class CBbRandom : public CBotBehaviorBase
{
public:
    void Initialize(game::SGameState const& /*gameState*/, SBotBehaviorInitParams const& /*initParams*/) override {}
    SVec2 FindTurn(game::SGameState const& gameState) override;
    void Shutdown() override {}
};

} // bot
} // ut3