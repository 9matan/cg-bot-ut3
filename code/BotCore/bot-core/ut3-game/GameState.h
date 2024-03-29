#pragma once

#include <bitset>
#include <cassert>

namespace ut3 {
namespace game {
#   define UT3_FIRST_BITS_MASK(bitsCount)  ((~(1ULL)) >> (64 - bitsCount))

#   define UT3_SET_BIT(number, bitPos, bitVal) number = ((number & (~(1ULL << (bitPos)))) | ((unsigned long long)((bitVal) == 0 ? 0 : 1) << (bitPos)))
#   define UT3_GET_BIT(number, bitPos) (((number) >> (bitPos)) & 1)

#   define UT3_SET_BITS(number, bitStartPos, bitsCount, bitsVal) number = ((number & (~(UT3_FIRST_BITS_MASK((unsigned long long)(bitsCount)) << (bitStartPos)))) | (((unsigned long long)(bitsVal)) << (bitStartPos)))
#   define UT3_GET_BITS(number, bitStartPos, bitsCount) (((number) >> (bitStartPos)) & UT3_FIRST_BITS_MASK((unsigned long long)(bitsCount)))

#   define UT3_COUNT_BITS_32(number) (std::bitset<32>(number).count())
#   define UT3_COUNT_BITS_64(number) (std::bitset<64>(number).count())

    // 0 - X, 1 - O
#   define GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(playerIndex) (1ULL << (playerIndex))
#   define GAME_STATE_PLAYER_ELEMENT_TO_PLAYER_INDEX(playerElement) ((playerElement) >> 1)

#   define GAME_STATE_ELEMENT_EMPTY 0
#   define GAME_STATE_ELEMENT_X 1
#   define GAME_STATE_ELEMENT_O 2
    // draw is used in global block. It shows that a block is full and there is no winner
#   define GAME_STATE_ELEMENT_DRAW 3

    // 0 - X, 1 - O
#   define GAME_STATE_SET_PLAYER(gameState, playerIndex) UT3_SET_BIT(gameState.m_data[2], 63, (playerIndex))
#   define GAME_STATE_GET_PLAYER(gameState) UT3_GET_BIT((gameState).m_data[2], 63)
#   define GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) (UT3_GET_BIT((gameState).m_data[2], 36 + (2 * blockIndex)) | (UT3_GET_BIT((gameState).m_data[2], 36 + 1 + (2 * blockIndex)) << 1))
#   define GAME_STATE_SET_BLOCK_STATUS(gameState, blockIndex, value) do \
    { \
        UT3_SET_BIT(gameState.m_data[2], 36 + (2 * blockIndex), value & 1); \
        UT3_SET_BIT(gameState.m_data[2], 36 + 1 + (2 * blockIndex), value & 2); \
    } while(false)

#   define GAME_STATE_SET_LAST_OPP_TURN_X(gameState, value) UT3_SET_BITS(gameState.m_data[2], 54, 4, (value))
#   define GAME_STATE_SET_LAST_OPP_TURN_Y(gameState, value) UT3_SET_BITS(gameState.m_data[2], 58, 4, (value))
#   define GAME_STATE_GET_LAST_OPP_TURN_X(gameState) UT3_GET_BITS((gameState).m_data[2], 54, 4)
#   define GAME_STATE_GET_LAST_OPP_TURN_Y(gameState) UT3_GET_BITS((gameState).m_data[2], 58, 4)
#   define GAME_STATE_INVALIDATE_LAST_OPP_TURN(gameState) UT3_SET_BITS((gameState).m_data[2], 54, 4, 15)
#   define GAME_STATE_IS_VALID_LAST_OPP_TURN(gameState) (UT3_GET_BITS((gameState).m_data[2], 54, 4) != 15)

#   define GAME_STATE_SET_GAME_WINNER(gameState, value) UT3_SET_BITS(gameState.m_data[1], 62, 2, (value))
#   define GAME_STATE_GET_GAME_WINNER(gameState) UT3_GET_BITS((gameState).m_data[1], 62, 2)

#   define GAME_BLOCK_STATE_POS_TO_INDEX(posX, posY) ((posX) + (posY) * 3)
#   define GAME_BLOCK_STATE_SET_ELEMENT(blockState, index, value) do \
    { \
        UT3_SET_BIT(blockState.m_data, 2 * index, (value & GAME_STATE_ELEMENT_X)); \
        UT3_SET_BIT(blockState.m_data, 2 * index + 1, (value & GAME_STATE_ELEMENT_O)); \
    } while(false)
#   define GAME_BLOCK_STATE_SET_ELEMENT_BY_POS(blockState, posX, posY, value) GAME_BLOCK_STATE_SET_ELEMENT(blockState, GAME_BLOCK_STATE_POS_TO_INDEX(posX, posY), value)
#   define GAME_BLOCK_STATE_GET_ELEMENT(blockState, index) (((blockState).m_data >> (2 * index)) & 3ULL)
#   define GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(blockState, posX, posY) GAME_BLOCK_STATE_GET_ELEMENT(blockState, GAME_BLOCK_STATE_POS_TO_INDEX(posX, posY))

#   define GAME_STATE_GET_GLOBAL_BLOCK(gameState) ut3::game::SGameBlockState{ ((gameState).m_data[2] >> 36) & UT3_FIRST_BITS_MASK(18) }

    constexpr unsigned int BLOCK_PLAYER_ELEMENTS_MASKS[2] = {
        87381,
        174762
    };

#   define GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(blockState, playerIndex) ((int)UT3_COUNT_BITS_32((blockState).m_data & ut3::game::BLOCK_PLAYER_ELEMENTS_MASKS[(playerIndex)]) - (int)GAME_STATE_BLOCK_COUNT_DRAW_ELEMENTS(blockState))
#   define GAME_STATE_BLOCK_COUNT_DRAW_ELEMENTS(blockState) UT3_COUNT_BITS_32(((blockState).m_data) & (((blockState).m_data & ut3::game::BLOCK_PLAYER_ELEMENTS_MASKS[1]) >> 1))
#   define GAME_STATE_ELEMENTS_COUNT(gameState) (UT3_COUNT_BITS_64((gameState).m_data[0]) + UT3_COUNT_BITS_64((gameState).m_data[1] & UT3_FIRST_BITS_MASK(62)) + UT3_COUNT_BITS_64((gameState).m_data[2] & UT3_FIRST_BITS_MASK(36)))

    struct SGameState
    {
        SGameState()
        {
            m_data[0] = m_data[1] = m_data[2] = 0;
            GAME_STATE_INVALIDATE_LAST_OPP_TURN(*this);
        }
        // the first 162 bits describe elements (2 bits per element)
        //      00 - empty
        //      10 - X
        //      01 - O
        //      0..63 bits from m_data[0]
        //      0..61 bits from m_data[1]
        //      0..35 bits from m_data[2]
        //          block 0: 0..17 from m_data[0]
        //          block 1: 18..35 from m_data[0]
        //          block 2: 36..53 from m_data[0]
        //          block 3: 54..63 from m_data[0] | 0..7 from m_data[1]
        //          block 4: 8..25 from m_data[1]
        //          block 5: 26..43 from m_data[1]
        //          block 6: 44..61 from m_data[1]
        //          block 7: 0..17 from m_data[2]
        //          block 8: 18..35 from m_data[2]
        // the next 18 bits describe status of each block (2 bits per block)
        //      0 - has empty cells, 1 - X won, 2 - O won, 3 - draw
        //      36..53 bits from m_data[2]
        // the next 8 bits describe the last opponent turn (4 bits per component)
        //      54..61 bits from m_data[2]
        //          54..57 for x
        //          58..61 for y
        // the bits 62..63 from m_data[1] describes who won the game
        // the last bit - who makes a turn (0 - X, 1 - O)
        //      63th bit from m_data[2]
        unsigned long long m_data[3];
    };

    static_assert(sizeof(SGameState) == 8 * 3, "SGameState has invalid size");

    struct SGameBlockState
    {
        // the first 18 bits describe elements (2 bits per element)
        //      00 - empty
        //      10 - X
        //      01 - O
        //      11 - Draw - for global block
        unsigned int m_data = 0;

        inline bool operator==(SGameBlockState other) const { return m_data == other.m_data; }
    };

    static_assert(sizeof(SGameBlockState) == 4, "SGameBlockState has invalid size");

    inline SGameBlockState GetBlockState(SGameState const gameState, size_t const blockIndex)
    {
        assert(blockIndex < 9);
        if (blockIndex < 3)
        {
            return SGameBlockState{ UT3_GET_BITS(gameState.m_data[0], blockIndex * 18, 18) };
        }
        if (blockIndex >= 4)
        {
            if (blockIndex < 7)
            {
                return SGameBlockState{ UT3_GET_BITS(gameState.m_data[1], (blockIndex - 4) * 18 + 8, 18) };
            }
            else
            {
                return SGameBlockState{ UT3_GET_BITS(gameState.m_data[2], (blockIndex - 7) * 18, 18) };
            }
        }
        return SGameBlockState{ (gameState.m_data[0] >> 54) | ((gameState.m_data[1] & UT3_FIRST_BITS_MASK(8)) << 10) };
    }

    inline void SetBlockState(SGameState& gameState, size_t const blockIndex, SGameBlockState const blockState)
    {
        assert(blockIndex < 9);
        if (blockIndex < 3)
        {
            UT3_SET_BITS(gameState.m_data[0], blockIndex * 18, 18, blockState.m_data);
        }
        else if (blockIndex >= 4)
        {
            if (blockIndex < 7)
            {
                UT3_SET_BITS(gameState.m_data[1], (blockIndex - 4) * 18 + 8, 18, blockState.m_data);
            }
            else
            {
                UT3_SET_BITS(gameState.m_data[2], (blockIndex - 7) * 18, 18, blockState.m_data);
            }
        }
        else
        {
            unsigned long long const firstBits = (blockState.m_data & UT3_FIRST_BITS_MASK(10));
            unsigned long long const secondBits = UT3_GET_BITS(blockState.m_data, 10, 8);
            UT3_SET_BITS(gameState.m_data[0], 54, 10, firstBits);
            UT3_SET_BITS(gameState.m_data[1], 0, 8, secondBits);
        }
    }
}
}