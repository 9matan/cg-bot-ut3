#include "BotCore_PCH.h"

#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameStateHelper.h"

namespace ut3 {
namespace game {

    char ConvertGameStateElemToChar(int const elem)
    {
        if (elem == GAME_STATE_ELEMENT_EMPTY) return '-';
        else if (elem == GAME_STATE_ELEMENT_X) return 'X';
        else if (elem == GAME_STATE_ELEMENT_O) return 'O';
        return 'D';
    }

    int ConvertCharToGameStateElem(char const c)
    {
        if (c == '-') return GAME_STATE_ELEMENT_EMPTY;
        else if (c == 'X') return GAME_STATE_ELEMENT_X;
        else if (c == 'O') return GAME_STATE_ELEMENT_O;
        return GAME_STATE_ELEMENT_DRAW;
    }

    SGameState CreateGameState(char const map[9][10], SVec2 const oppLastTurn)
    {
        SGameState gameState;

        size_t const elementsCount = GAME_STATE_ELEMENTS_COUNT(gameState);
        size_t const palyerIndex = (elementsCount & 1) ? 1 : 0;

        GAME_STATE_SET_PLAYER(gameState, palyerIndex);
        GAME_STATE_SET_LAST_OPP_TURN_X(gameState, oppLastTurn[0]);
        GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, oppLastTurn[1]);

        for (int blockX = 0; blockX < 3; ++blockX)
        {
            for (int blockY = 0; blockY < 3; ++blockY)
            {
                SGameBlockState blockState;
                for (int x = 0; x < 3; ++x)
                {
                    for (int y = 0; y < 3; ++y)
                    {
                        int const elem = ConvertCharToGameStateElem(map[y + 3 * blockY][x + 3 * blockX]);
                        GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, x, y, elem);
                    }
                }

                int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);
                SetBlockState(gameState, blockIndex, blockState);
                int const blockWinner = CalculateWinner(blockState);
                GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, blockWinner);
            }
        }

        int const gameWinner = CalculateWinner(gameState);
        GAME_STATE_SET_GAME_WINNER(gameState, gameWinner);

        return gameState;
    }

} // game
} // ut3