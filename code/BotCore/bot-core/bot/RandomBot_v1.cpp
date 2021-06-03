#include "BotCore_PCH.h"
#include "bot-core/bot/RandomBot_v1.h"

#include "mimax/common/Random.h"

#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameStateView.h"

namespace ut3 {
namespace bot {

    SVec2 CRandomBot_v1::FindTurn(game::SGameState const& gameState)
    {
        game::Turns turns;
        game::CollectPossibleTurns(gameState, turns);
        return *mimax::common::GetRandomItem(turns.begin(), turns.end());
    }

}
}