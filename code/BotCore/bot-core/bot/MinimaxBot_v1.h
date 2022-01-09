#pragma once

#include "mimax/common/Vec2.h"

#include "bot-core/bot/BotBase.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace bot {

    class CMinimaxBot_v1 : public CBotBase
    {
    public:
        static SVec2 FindTurn(game::SGameState const& gameState, int const myPlayer, bool const debugEnabled = false);
    
    public:
        CMinimaxBot_v1();
    
    protected:
        SVec2 FindTurn(game::SGameState const& gameState) override;
    };

}
}