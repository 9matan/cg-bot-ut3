#include "BotCore_PCH.h"

#include "bot-core/ut3-game/Game.h"

namespace
{
    // masks si computed for X player. To get O - shift it left for a one bit
    constexpr size_t WINNER_MASKS_COUNT = 8;
    constexpr unsigned int WINNER_MASKS[WINNER_MASKS_COUNT] = {
        21, // first row
        1344, // second row
        86016, // third row
        4161, // first column
        16644, // second column
        66576, // third column
        65793, // main diagonal
        4368// antidiagonal
    };

#   define GET_WINNER_MASK_FOR_PLAYER(mask, playerIndex) ((mask) << (playerIndex))

    inline void AddAvailableTurns(ut3::game::Turns& turnsOut, ut3::game::SGameBlockState block, int const blockX, int const blockY)
    {
        for (int x = 0; x < 3; ++x)
        {
            for (int y = 0; y < 3; ++y)
            {
                if (GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(block, x, y) == GAME_STATE_ELEMENT_EMPTY)
                {
                    turnsOut.push_back({x + 3 * blockX, y + 3 * blockY});
                }
            }
        }
    }
}

namespace ut3 {
namespace game {

    int CalculateWinner(SGameState const& gameState)
    {
        return CalculateWinner(GAME_STATE_GET_GLOBAL_BLOCK(gameState));
    }

    int CalculateWinner(SGameBlockState const blockState)
    {
        for (int i = 0; i < WINNER_MASKS_COUNT; ++i)
        {
            unsigned int const mask0 = GET_WINNER_MASK_FOR_PLAYER(WINNER_MASKS[i], 0);
            unsigned int const mask1 = GET_WINNER_MASK_FOR_PLAYER(WINNER_MASKS[i], 1);
            if ((mask0 & blockState.m_data) == mask0)
            {
                // to ignore draw elements
                if (UT3_COUNT_BITS_32(mask1 & blockState.m_data) == 0)
                    return GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(0);
            }
            else if ((mask1 & blockState.m_data) == mask1)
            {
                // to ignore draw elements
                if (UT3_COUNT_BITS_32(mask0 & blockState.m_data) == 0)
                    return GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(1);
            }
        }

        for (int i = 0; i < 9; ++i)
        {
            if (GAME_BLOCK_STATE_GET_ELEMENT(blockState, i) == GAME_STATE_ELEMENT_EMPTY)
                return GAME_STATE_ELEMENT_EMPTY;
        }

        return GAME_STATE_ELEMENT_DRAW;
    }

    void MakeTurn(SGameState& gameState, int const worldX, int const worldY)
    {
        int const blockX = worldX / 3;
        int const blockY = worldY / 3;
        int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);
        auto block = GetBlockState(gameState, blockIndex);

        int const localX = worldX - 3 * blockX;
        int const localY = worldY - 3 * blockY;
        int const playerIndex = GAME_STATE_GET_PLAYER(gameState);

        GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(block, localX, localY, GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(playerIndex));
        SetBlockState(gameState, blockIndex, block);

        GAME_STATE_SET_PLAYER(gameState, ((playerIndex + 1) & 1));
        GAME_STATE_SET_LAST_OPP_TURN_X(gameState, worldX);
        GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, worldY);

        int const blockWinner = CalculateWinner(block);
        if (blockWinner != GAME_STATE_ELEMENT_EMPTY)
        {
            GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, blockWinner);
            int const gameWinner = CalculateWinner(gameState);
            GAME_STATE_SET_GAME_WINNER(gameState, gameWinner);
        }
    }

    void CollectPossibleTurns(SGameState const& gameState, Turns& turnsOut)
    {
        turnsOut.clear();
        if (GAME_STATE_GET_GAME_WINNER(gameState) != GAME_STATE_ELEMENT_EMPTY)
            return;

        bool findAllTurns = true;
        if (GAME_STATE_IS_VALID_LAST_OPP_TURN(gameState))
        {
            int const blockX = GAME_STATE_GET_LAST_OPP_TURN_X(gameState) % 3;
            int const blockY = GAME_STATE_GET_LAST_OPP_TURN_Y(gameState) % 3;
            int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);

            if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY)
            {
                findAllTurns = false;
                auto const block = GetBlockState(gameState, blockIndex);
                AddAvailableTurns(turnsOut, block, blockX, blockY);
            }
        }

        if (findAllTurns)
        {
            for (int blockX = 0; blockX < 3; ++blockX)
            {
                for (int blockY = 0; blockY < 3; ++blockY)
                {
                    int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);
                    if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY)
                    {
                        auto const block = GetBlockState(gameState, blockIndex);
                        AddAvailableTurns(turnsOut, block, blockX, blockY);
                    }
                }
            }
        }
    }
}
}