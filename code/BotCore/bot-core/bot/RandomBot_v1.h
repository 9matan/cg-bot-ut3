#pragma once

#include "bot-core/bot/BotBase.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace bot {

    class CRandomBot_v1 : public CBotBase
    {
    public:
        char const* GetBotName() const { return "CRandomBot_v1"; }
    protected:
        SVec2 FindTurn(game::SGameState const& gameState) override;
    };

}
}