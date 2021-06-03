#include "BotCore_PCH.h"
#include "bot-core/ut3-game/GameHelper.h"

#include "bot-core/ut3-game/Game.h"

namespace ut3 {
namespace game {

    SGameState PlayGame(std::array<FindTurnFunc, 2> bots, SGameState gameState)
    {
        int currentBotIndex = 0;
        while (GAME_STATE_GET_GAME_WINNER(gameState) == GAME_STATE_ELEMENT_EMPTY)
        {
            SVec2 const turn = bots[currentBotIndex](gameState);

            Turns turns;
            CollectPossibleTurns(gameState, turns);
            assert(std::find(turns.begin(), turns.end(), turn) != turns.end());

            MakeTurn(gameState, turn[0], turn[1]);
            currentBotIndex = ((currentBotIndex + 1) & 1);
        }

        return gameState;
    }

}
}