
#include "mimax/common/Random.h"

#include "bot-core/ut3-game/GameHelper.h"

#include "bot-core-test/playground/BotsPlayground.h"

namespace ut3
{
    class CFindNextTurn
    {
    public:
        CFindNextTurn(IBot* bot) : m_bot(bot) {}

        SVec2 operator()(game::SGameState const& gameState)
        {
            SInputData inputData;
            if (GAME_STATE_IS_VALID_LAST_OPP_TURN(gameState))
            {
                inputData.m_oppTurnX = GAME_STATE_GET_LAST_OPP_TURN_X(gameState);
                inputData.m_oppTurnY = GAME_STATE_GET_LAST_OPP_TURN_Y(gameState);
            }
            else
            {
                inputData.m_oppTurnX = inputData.m_oppTurnY = -1;
            }

            bool const isFirstUpdate = GAME_STATE_ELEMENTS_COUNT(gameState) <= 1;
            auto outputData = isFirstUpdate
                ? m_bot->FirstUpdate(inputData)
                : m_bot->Update(inputData);
            return { outputData.m_turnX, outputData.m_turnY };
        }

    private:
        IBot* m_bot;
    };

    game::SGameState PlayGame(std::array<IBot*, 2> bots)
    {
        bots[0]->SetDebugIsEnabled(false);
        bots[1]->SetDebugIsEnabled(false);

        return game::PlayGame({ CFindNextTurn(bots[0]), CFindNextTurn(bots[1]) });
    }


    CPlayground::SMatchStatistics CPlayground::UpdateStats(game::SGameState const& gameState, int const xPlayerBotIndex)
    {
        SMatchStatistics matchStats;
        matchStats.m_winner = GAME_STATE_GET_GAME_WINNER(gameState);
        if (matchStats.m_winner == GAME_STATE_ELEMENT_DRAW)
        {
            matchStats.m_winnerBotIndex = -1;
            ++m_stats.m_drawsCount;
        }
        else
        {
            if (matchStats.m_winner == GAME_STATE_ELEMENT_X)
            {
                matchStats.m_winnerBotIndex = xPlayerBotIndex;
                ++m_stats.m_winsCount[0][matchStats.m_winnerBotIndex];
            }
            else
            {
                matchStats.m_winnerBotIndex = ((xPlayerBotIndex + 1) & 1);
                ++m_stats.m_winsCount[1][(matchStats.m_winnerBotIndex)];
            }
        }
        return matchStats;
    }
}