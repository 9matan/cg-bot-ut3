#include <cassert>

#include "bot-core/ut3-game/GameStateHelper.h"

namespace ut3 {
namespace game {

    void RunTests_GameStateHelper_CreateGameState()
    {
        {
            char const map[9][10] = {
                "O--------",
                "-X-------",
                "--O------",
                "---X-----",
                "--O------",
                "-X-------",
                "O-X------",
                "---O-----",
                "----X----"
            };
            auto const gameState = CreateGameState(
                map, {2, 4}
            );

            assert(GAME_STATE_ELEMENTS_COUNT(gameState) == 10);
            assert(GAME_STATE_GET_PLAYER(gameState) == 0);
            assert(GAME_STATE_GET_GAME_WINNER(gameState) == GAME_STATE_ELEMENT_EMPTY);
        }

        {
            char const map[9][10] = {
                "-X-------",
                "-X-------",
                "-X-------",
                "---------",
                "---------",
                "------OOO",
                "OXX------",
                "XXO------",
                "OOX------"
            };
            auto const gameState = CreateGameState(
                map, { 1, 7 }
            );

            assert(GAME_STATE_GET_BLOCK_STATUS(gameState, 0) == GAME_STATE_ELEMENT_X);
            assert(GAME_STATE_GET_BLOCK_STATUS(gameState, 4) == GAME_STATE_ELEMENT_EMPTY);
            assert(GAME_STATE_GET_BLOCK_STATUS(gameState, 5) == GAME_STATE_ELEMENT_O);
            assert(GAME_STATE_GET_BLOCK_STATUS(gameState, 6) == GAME_STATE_ELEMENT_DRAW);
        }
    }

    void RunTests_GameStateHelper()
    {
        RunTests_GameStateHelper_CreateGameState();
    }
}
}