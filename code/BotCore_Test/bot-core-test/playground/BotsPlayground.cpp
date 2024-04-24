
#include "mimax/common/Random.h"

#include "bot-core/ut3-game/GameHelper.h"

#include "bot-core-test/playground/BotsPlayground.h"

namespace ut3 {
namespace bot {

    namespace
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
    }

    std::array<CPlayground::SBotPairStats, 2> CPlayground::RunPairPlayground(std::array<bot::IBot*, 2> const& bots, int const matchesCount)
    {
        OnPairPlaygroundStarted({ bots[0]->GetBotName(), bots[1]->GetBotName() });

        std::array<CPlayground::SBotPairStats, 2> stats;
        mimax::common::UpdateRandomSeed();
        IBot* xBot = bots[0];
        IBot* oBot = bots[1];
        for (int matchIndex = 0; matchIndex < matchesCount * 2; ++matchIndex)
        {
            OnMatchStart(matchIndex);
            for (auto bot : bots)
            {
                bot->Reset();
                bot->SetDebugIsEnabled(false);
            }

            auto const gameState = game::PlayGame({ CFindNextTurn(xBot), CFindNextTurn(oBot) });

            SMatchStats matchStats;
            {
                int const gameWinner = GAME_STATE_GET_GAME_WINNER(gameState);
                if (gameWinner == GAME_STATE_ELEMENT_DRAW)
                {
                    matchStats.m_botName = "None";
                    ++stats[0].m_drawsCount;
                    ++stats[1].m_drawsCount;
                }
                else
                {
                    int const playerIndex = GAME_STATE_PLAYER_ELEMENT_TO_PLAYER_INDEX(gameWinner);
                    int const botIndex = (((matchIndex & 1) + playerIndex) & 1);

                    ++stats[botIndex].m_winsCount[playerIndex];
                    matchStats.m_botName = bots[botIndex]->GetBotName();
                    matchStats.m_botName += "_";
                    matchStats.m_botName += std::to_string(botIndex);
                }
                matchStats.m_winnerPlayer = gameWinner;
            }

            OnMatchEnded(gameState, matchStats);

            std::swap(xBot, oBot);
        }
        return stats;
    }

    std::vector<CPlayground::SBotStats> CPlayground::RunPlayground(std::vector<bot::IBot*> const& bots, int const matchesCountPerPair)
    {
        assert(bots.size() >= 2);
        OnPlaygroundStarted();

        std::vector<CPlayground::SBotStats> stats;
        stats.resize(bots.size());
        for (int botIndex = 0; botIndex < bots.size(); ++botIndex)
        {
            stats[botIndex].m_botName = bots[botIndex]->GetBotName();
        }

        for (int firstBotIndex = 0; firstBotIndex < bots.size(); ++firstBotIndex)
        {
            for (int secondBotIndex = firstBotIndex + 1; secondBotIndex < bots.size(); ++secondBotIndex)
            {
                auto const pairStats = RunPairPlayground({ bots[firstBotIndex], bots[secondBotIndex] }, matchesCountPerPair);

                stats[firstBotIndex].AddPairEntry(pairStats[0], pairStats[1], bots[secondBotIndex]->GetBotName());
                stats[secondBotIndex].AddPairEntry(pairStats[1], pairStats[0], bots[firstBotIndex]->GetBotName());
            }
        }

        OnPlaygroundEnded(stats);
        return stats;
    }
}
}