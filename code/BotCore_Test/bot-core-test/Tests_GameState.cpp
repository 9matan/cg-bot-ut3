#include <cassert>

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace game {

    void RunTests_GameState_GlobalData()
    {
        {
            SGameState state;

            GAME_STATE_SET_PLAYER(state, 1);

            assert(GAME_STATE_GET_PLAYER(state) == 1);
        }
        {
            SGameState state;

            GAME_STATE_SET_PLAYER(state, 1);
            GAME_STATE_SET_PLAYER(state, 0);

            assert(GAME_STATE_GET_PLAYER(state) == 0);
        }
        {
            SGameState state;

            GAME_STATE_SET_LAST_OPP_TURN_X(state, 5);
            GAME_STATE_SET_LAST_OPP_TURN_Y(state, 8);

            assert(GAME_STATE_GET_LAST_OPP_TURN_X(state) == 5);
            assert(GAME_STATE_GET_LAST_OPP_TURN_Y(state) == 8);
        }
        {
            SGameState state;

            GAME_STATE_SET_GAME_WINNER(state, GAME_STATE_ELEMENT_O);

            assert(GAME_STATE_GET_GAME_WINNER(state) == GAME_STATE_ELEMENT_O);
        }
        {
            SGameState state;

            GAME_STATE_SET_BLOCK_STATUS(state, 0, GAME_STATE_ELEMENT_DRAW);
            GAME_STATE_SET_BLOCK_STATUS(state, 1, GAME_STATE_ELEMENT_O);
            GAME_STATE_SET_BLOCK_STATUS(state, 2, GAME_STATE_ELEMENT_X);

            assert(GAME_STATE_GET_BLOCK_STATUS(state, 0) == GAME_STATE_ELEMENT_DRAW);
            assert(GAME_STATE_GET_BLOCK_STATUS(state, 1) == GAME_STATE_ELEMENT_O);
            assert(GAME_STATE_GET_BLOCK_STATUS(state, 2) == GAME_STATE_ELEMENT_X);
        }
        {
            SGameState state;

            GAME_STATE_SET_BLOCK_STATUS(state, 6, GAME_STATE_ELEMENT_DRAW);
            GAME_STATE_SET_BLOCK_STATUS(state, 7, GAME_STATE_ELEMENT_O);
            GAME_STATE_SET_BLOCK_STATUS(state, 8, GAME_STATE_ELEMENT_X);

            assert(GAME_STATE_GET_BLOCK_STATUS(state, 6) == GAME_STATE_ELEMENT_DRAW);
            assert(GAME_STATE_GET_BLOCK_STATUS(state, 7) == GAME_STATE_ELEMENT_O);
            assert(GAME_STATE_GET_BLOCK_STATUS(state, 8) == GAME_STATE_ELEMENT_X);
        }
        {
            SGameState state;
            GAME_STATE_SET_BLOCK_STATUS(state, 4, GAME_STATE_ELEMENT_DRAW);
            GAME_STATE_SET_BLOCK_STATUS(state, 5, GAME_STATE_ELEMENT_DRAW);
            GAME_STATE_SET_BLOCK_STATUS(state, 6, GAME_STATE_ELEMENT_DRAW);

            GAME_STATE_SET_BLOCK_STATUS(state, 5, GAME_STATE_ELEMENT_EMPTY);

            assert(GAME_STATE_GET_BLOCK_STATUS(state, 4) == GAME_STATE_ELEMENT_DRAW);
            assert(GAME_STATE_GET_BLOCK_STATUS(state, 5) == GAME_STATE_ELEMENT_EMPTY);
            assert(GAME_STATE_GET_BLOCK_STATUS(state, 6) == GAME_STATE_ELEMENT_DRAW);
        }
        {
            assert(GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(0) == GAME_STATE_ELEMENT_X);
            assert(GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(1) == GAME_STATE_ELEMENT_O);
        }
    }

    void RunTests_GameState_GetSetBlockElement()
    {
        {
            assert(GAME_BLOCK_STATE_POS_TO_INDEX(0, 0) == 0);
            assert(GAME_BLOCK_STATE_POS_TO_INDEX(1, 0) == 1);
            assert(GAME_BLOCK_STATE_POS_TO_INDEX(2, 0) == 2);

            assert(GAME_BLOCK_STATE_POS_TO_INDEX(0, 1) == 3);
            assert(GAME_BLOCK_STATE_POS_TO_INDEX(1, 1) == 4);
            assert(GAME_BLOCK_STATE_POS_TO_INDEX(2, 1) == 5);

            assert(GAME_BLOCK_STATE_POS_TO_INDEX(0, 2) == 6);
            assert(GAME_BLOCK_STATE_POS_TO_INDEX(1, 2) == 7);
            assert(GAME_BLOCK_STATE_POS_TO_INDEX(2, 2) == 8);
        }
        {
            SGameBlockState state;

            GAME_BLOCK_STATE_SET_ELEMENT(state, 5, GAME_STATE_ELEMENT_X);

            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 5) == GAME_STATE_ELEMENT_X);
        }
        {
            SGameBlockState state;

            GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(state, 1, 2, GAME_STATE_ELEMENT_O);

            assert(GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(state, 1, 2) == GAME_STATE_ELEMENT_O);
        }
        {
            SGameBlockState state;

            GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(state, 1, 1, GAME_STATE_ELEMENT_DRAW);
            GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(state, 2, 1, GAME_STATE_ELEMENT_O);
            GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(state, 0, 2, GAME_STATE_ELEMENT_X);

            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 4) == GAME_STATE_ELEMENT_DRAW);
            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 5) == GAME_STATE_ELEMENT_O);
            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 6) == GAME_STATE_ELEMENT_X);
        }
        {
            SGameBlockState state;

            GAME_BLOCK_STATE_SET_ELEMENT(state, 0, GAME_STATE_ELEMENT_X);
            GAME_BLOCK_STATE_SET_ELEMENT(state, 1, GAME_STATE_ELEMENT_DRAW);
            GAME_BLOCK_STATE_SET_ELEMENT(state, 2, GAME_STATE_ELEMENT_O);

            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 0) == GAME_STATE_ELEMENT_X);
            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 1) == GAME_STATE_ELEMENT_DRAW);
            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 2) == GAME_STATE_ELEMENT_O);
        }
        {
            SGameBlockState state;
            GAME_BLOCK_STATE_SET_ELEMENT(state, 2, GAME_STATE_ELEMENT_DRAW);
            GAME_BLOCK_STATE_SET_ELEMENT(state, 3, GAME_STATE_ELEMENT_DRAW);
            GAME_BLOCK_STATE_SET_ELEMENT(state, 4, GAME_STATE_ELEMENT_DRAW);

            GAME_BLOCK_STATE_SET_ELEMENT(state, 3, GAME_STATE_ELEMENT_EMPTY);

            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 2) == GAME_STATE_ELEMENT_DRAW);
            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 3) == GAME_STATE_ELEMENT_EMPTY);
            assert(GAME_BLOCK_STATE_GET_ELEMENT(state, 4) == GAME_STATE_ELEMENT_DRAW);
        }
        {
            SGameState gameState;
            SGameBlockState blockState;
            GAME_STATE_SET_BLOCK_STATUS(gameState, 1, GAME_STATE_ELEMENT_X);
            GAME_STATE_SET_BLOCK_STATUS(gameState, 3, GAME_STATE_ELEMENT_DRAW);
            GAME_STATE_SET_BLOCK_STATUS(gameState, 8, GAME_STATE_ELEMENT_O);

            blockState = GAME_STATE_GET_GLOBAL_BLOCK(gameState);

            assert(GAME_BLOCK_STATE_GET_ELEMENT(blockState, 1) == GAME_STATE_ELEMENT_X);
            assert(GAME_BLOCK_STATE_GET_ELEMENT(blockState, 3) == GAME_STATE_ELEMENT_DRAW);
            assert(GAME_BLOCK_STATE_GET_ELEMENT(blockState, 8) == GAME_STATE_ELEMENT_O);
        }
        {
            SGameBlockState blockState;
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 2, GAME_STATE_ELEMENT_DRAW);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 3, GAME_STATE_ELEMENT_DRAW);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 4, GAME_STATE_ELEMENT_DRAW);

            assert(GAME_STATE_BLOCK_COUNT_DRAW_ELEMENTS(blockState) == 3);
        }
        {
            SGameBlockState blockState;
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 0, GAME_STATE_ELEMENT_X);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 2, GAME_STATE_ELEMENT_O);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 3, GAME_STATE_ELEMENT_O);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 4, GAME_STATE_ELEMENT_DRAW);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 7, GAME_STATE_ELEMENT_X);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 8, GAME_STATE_ELEMENT_X);

            assert(GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(blockState, 0) == 3);
            assert(GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(blockState, 1) == 2);
        }
    }

    void RunTests_GameState_GetSetBlock()
    {
        {
            SGameBlockState blockState;
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 1, GAME_STATE_ELEMENT_X);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 2, GAME_STATE_ELEMENT_DRAW);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 3, GAME_STATE_ELEMENT_O);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 5, GAME_STATE_ELEMENT_O);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 7, GAME_STATE_ELEMENT_X);
            GAME_BLOCK_STATE_SET_ELEMENT(blockState, 8, GAME_STATE_ELEMENT_X);
            for(int i = 0; i < 9; ++i)
            {
                SGameState gameState;

                SetBlockState(gameState, i, blockState);

                assert(GetBlockState(gameState, i) == blockState);
            }
        }
    }

    void RunTests_GameState()
    {
        RunTests_GameState_GlobalData();
        RunTests_GameState_GetSetBlockElement();
        RunTests_GameState_GetSetBlock();
    }
}
}