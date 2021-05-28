#include "BotCore_PCH.h"

#include "bot-core/ut3-game/GameStateView.h"

namespace ut3 {
namespace game {

    inline char ConvertGameStateElemToChar(int const elem)
    {
        if (elem == GAME_STATE_ELEMENT_EMPTY) return '-';
        else if (elem == GAME_STATE_ELEMENT_X) return 'X';
        else if (elem == GAME_STATE_ELEMENT_O) return 'O';
        return 'D';
    }

    SGameStateView::SGameStateView(SGameState const& state)
    {
        for (int blockX = 0; blockX < 3; ++blockX)
        {
            for (int blockY = 0; blockY < 3; ++blockY)
            {
                int const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);

                m_globalBlockStatuses[blockY][blockX] = ConvertGameStateElemToChar(
                    GAME_STATE_GET_BLOCK_STATUS(state, blockIndex));

                auto const block = GetBlockState(state, blockIndex);

                for (int x = 0; x < 3; ++x)
                {
                    for (int y = 0; y < 3; ++y)
                    {
                        m_map[y + blockY * 3][x + blockX * 3] = ConvertGameStateElemToChar(
                            GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(block, x, y));
                    }
                }
            }
        }

        for (int r = 0; r < 3; ++r)
        {
            m_globalBlockStatuses[r][3] = '\0';
        }

        for (int r = 0; r < 9; ++r)
        {
            m_map[r][9] = '\0';
        }

        m_player = ConvertGameStateElemToChar(
            GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(
                GAME_STATE_GET_PLAYER(state)));
        m_oppLastTurnX = GAME_STATE_GET_LAST_OPP_TURN_X(state);
        m_oppLastTurnY = GAME_STATE_GET_LAST_OPP_TURN_Y(state);
        m_gameWinner = ConvertGameStateElemToChar(
            GAME_STATE_GET_GAME_WINNER(state));

        auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);
        m_xBlocksCnt = (unsigned char)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 0);
        m_oBlocksCnt = (unsigned char)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 1);
        m_elementsCnt = (unsigned char)GAME_STATE_ELEMENTS_COUNT(state);
    }

    void SGameStateView::Print()
    {
        for (int r = 0; r < 9; ++r)
        {
            std::cerr << m_map[r] << "\n";
        }
        for (int r = 0; r < 3; ++r)
        {
            std::cerr << m_globalBlockStatuses[r] << "\n";
        }
        std::cerr << "Player: " << m_player << "\n";
        std::cerr << "Opp last turn: " << (int)m_oppLastTurnY << ", " << (int)m_oppLastTurnX << "\n";
        std::cerr << "Game winner: " << m_gameWinner << "\n";
        std::cerr << "X blocks count: " << (int)m_xBlocksCnt << "\n";
        std::cerr << "O blocks count: " << (int)m_oBlocksCnt << "\n";
        std::cerr << "Elements count: " << (int)m_elementsCnt << "\n";
    }

}
}