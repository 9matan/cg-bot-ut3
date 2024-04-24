
#include <cassert>

#include "bot-core/ut3-game/GameStateHelper.h"
#include "bot-core/bot/BbMinimax.h"

namespace ut3 {
namespace bot {

    void RunTests_MinimaxBot_v1()
    {
        {
            char const map[9][10] = {
                "-XOXO-XOX",
                "--O-X-OOX",
                "--O--XOXO",
                "XO-OOOXOX",
                "OX--X--O-",
                "XXO----O-",
                "-OXOOXXOX",
                "-OXXOOOOX",
                "XXXOXXXXO"
            };
            auto const gameState = game::CreateGameState(map, {0, 4});

            bot::CBbMinimax bb;
            bb.Initialize(gameState, bot::SBotBehaviorInitParams());
            assert(bb.FindTurn(gameState) == SVec2(2, 3));
        }
    }

    void RunTests_MinimaxBot()
    {
        RunTests_MinimaxBot_v1();
    }

}
}