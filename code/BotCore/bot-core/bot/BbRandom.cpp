#include "BotCore_PCH.h"
#include "bot-core/bot/BbRandom.h"

#include "mimax/common/Random.h"

#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameStateView.h"

namespace ut3 {
namespace bot {

SVec2 CBbRandom::FindTurn(game::SGameState const& gameState)
{
    mimax::common::UpdateRandomSeed();
    game::Turns turns;
    game::CollectPossibleTurns(gameState, turns);
    return *mimax::common::GetRandomItem(turns.begin(), turns.end());
}

} // bot
} // ut3