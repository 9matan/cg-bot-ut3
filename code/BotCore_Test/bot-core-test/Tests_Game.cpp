#include <cassert>

#include "bot-core/ut3-game/Game.h"

namespace ut3 {
namespace game {

    void RunTests_Game_CalculateWinner()
    {
        // there is a winner (game block state)
        {
            // rows
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 0, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 0, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 0, GAME_STATE_ELEMENT_X);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_X);
            }
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 1, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 1, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 1, GAME_STATE_ELEMENT_O);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_O);
            }
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 2, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 2, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 2, GAME_STATE_ELEMENT_X);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_X);
            }
            // columns
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 0, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 1, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 2, GAME_STATE_ELEMENT_O);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_O);
            }
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 0, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 1, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 2, GAME_STATE_ELEMENT_X);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_X);
            }
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 0, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 1, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 2, GAME_STATE_ELEMENT_O);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_O);
            }
            // diagonals
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 0, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 1, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 2, GAME_STATE_ELEMENT_X);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_X);
            }
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 2, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 1, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 0, GAME_STATE_ELEMENT_O);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_O);
            }
        }

        // there is no winner and there is available turns (game block state)
        {
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 0, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 0, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 0, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 1, GAME_STATE_ELEMENT_O);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_EMPTY);
            }
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 0, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 1, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 1, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 2, GAME_STATE_ELEMENT_O);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_EMPTY);
            }
        }

        // draw cases (game block state)
        {
            {
                SGameBlockState blockState;
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 0, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 0, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 0, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 1, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 1, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 1, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 0, 2, GAME_STATE_ELEMENT_X);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 1, 2, GAME_STATE_ELEMENT_O);
                GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, 2, 2, GAME_STATE_ELEMENT_X);

                assert(CalculateWinner(blockState) == GAME_STATE_ELEMENT_DRAW);
            }
        }
    
        // there is a winner (game state)
        {
            {
                SGameState gameState;
                GAME_STATE_SET_BLOCK_STATUS(gameState, 0, GAME_STATE_ELEMENT_O);
                GAME_STATE_SET_BLOCK_STATUS(gameState, 1, GAME_STATE_ELEMENT_O);
                GAME_STATE_SET_BLOCK_STATUS(gameState, 2, GAME_STATE_ELEMENT_O);

                assert(CalculateWinner(gameState) == GAME_STATE_ELEMENT_O);
            }
            {
                SGameState gameState;
                GAME_STATE_SET_BLOCK_STATUS(gameState, 0, GAME_STATE_ELEMENT_X);
                GAME_STATE_SET_BLOCK_STATUS(gameState, 4, GAME_STATE_ELEMENT_X);
                GAME_STATE_SET_BLOCK_STATUS(gameState, 8, GAME_STATE_ELEMENT_X);

                assert(CalculateWinner(gameState) == GAME_STATE_ELEMENT_X);
            }
        }
    }

    void RunTests_Game_MakeTurn()
    {
        using namespace ut3::game;
        // switch the player index
        {
            {
                SGameState gameState;

                MakeTurn(gameState, 3, 3);

                assert(GAME_STATE_GET_PLAYER(gameState) == 1);
            }
            {
                SGameState gameState;

                MakeTurn(gameState, 1, 1);
                MakeTurn(gameState, 4, 8);

                assert(GAME_STATE_GET_PLAYER(gameState) == 0);
            }
        }

        // block winner
        {
            SGameState gameState;

            MakeTurn(gameState, 3, 3);
            MakeTurn(gameState, 1, 1);
            MakeTurn(gameState, 5, 5);
            MakeTurn(gameState, 7, 7);
            MakeTurn(gameState, 4, 4);
            
            auto const block = GetBlockState(gameState, 4);
            assert(CalculateWinner(block) == GAME_STATE_ELEMENT_X);
        }
        // update the last opp turn
        {
            SGameState gameState;

            MakeTurn(gameState, 2, 5);

            assert(GAME_STATE_GET_LAST_OPP_TURN_X(gameState) == 2);
            assert(GAME_STATE_GET_LAST_OPP_TURN_Y(gameState) == 5);
        }
    }

    void RunTests_Game_CollectPossibleTurns()
    {
        using namespace ut3::game;
        {
            SGameState gameState;
            MakeTurn(gameState, 3, 3);
            MakeTurn(gameState, 1, 1);
            MakeTurn(gameState, 5, 5);
            MakeTurn(gameState, 7, 7);
            MakeTurn(gameState, 4, 4);

            Turns turns;
            CollectPossibleTurns(gameState, turns);

            assert(turns.size() == (9 * 9 - 3 * 3 - 2));
        }
        {
            SGameState gameState;

            Turns turns;
            CollectPossibleTurns(gameState, turns);

            assert(turns.size() == (9 * 9));
        }
        {
            SGameState gameState;
            MakeTurn(gameState, 2, 6);
            MakeTurn(gameState, 6, 2);

            Turns turns;
            CollectPossibleTurns(gameState, turns);

            assert(turns.size() == 8);
        }
    }

    void RunTests_Game()
    {
        RunTests_Game_CalculateWinner();
        RunTests_Game_MakeTurn();
        RunTests_Game_CollectPossibleTurns();
    }
}
}