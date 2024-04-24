#pragma once

#include <functional>
#include <string>

#include "bot-core/bot/BotBehaviorBase.h"

namespace ut3 {
namespace bot {

class CBbCustom : public CBotBehaviorBase
{
public:
    using FindTurnFunc = std::function<SVec2(game::SGameState const& gameState)>;

    CBbCustom(FindTurnFunc findTurnFunc);

    void Initialize(game::SGameState const& /*gameState*/, SBotBehaviorInitParams const& /*initParams*/) override {}
    SVec2 FindTurn(game::SGameState const& gameState) override;
    void Shutdown() override {}

private:
    FindTurnFunc m_findTurnFunc;
};

} // bot
} // ut3