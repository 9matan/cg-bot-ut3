#include "bot-core/ut3-game/GameStateView.h"

#include <vector>

#include "bot-core/ut3-game/GameHelper.h"

#include "bot-core/bot/CustomBot.h"
#include "bot-core/bot/MinimaxBot_v1.h"
#include "bot-core/bot/RandomBot_v1.h"

#include "bot-core-test/playground/BotsPlayground.h"

namespace ut3 {
namespace bot {

    namespace
    {
        class CConsoleBotsPlayground : public CPlayground
        {
        protected:
            void OnPlaygroundStarted() override
            {
                printf("=========================\n");
                printf("== PLAYGROUND ===========\n");
                printf("=========================\n");
                printf("\n");
            }

            void OnPairPlaygroundStarted(std::array<std::string, 2> const& botNames) override
            {
                printf("== PAIR PLAYGROUND: %s vs %s ===========\n", botNames[0].c_str(), botNames[1].c_str());
            }

            void OnMatchStart(int const matchIndex) override
            {
                printf("Match #%i:", matchIndex);
            }

            void OnMatchEnded(game::SGameState const& /*gameState*/, SMatchStats const& stats) override
            {
                printf(" %c (%s)\n", game::ConvertGameStateElemToChar(stats.m_winnerPlayer), stats.m_botName.c_str());
            }

            void OnPlaygroundEnded(std::vector<SBotStats> const& stats) override
            {
                printf("\n");
                printf("=========================\n");
                printf("== PLAYGROUND RESULTS ===\n");
                printf("=========================\n");

                for (auto const& botStats : stats)
                {
                    SBotStats::SPairEntry totalStats;
                    totalStats.m_oppBotName = "TOTAL";

                    printf("\n");
                    printf("=========================\n");
                    printf("== BOT: %s\n", botStats.m_botName.c_str());

                    for (auto const& pairStats : botStats.m_pairsStats)
                    {
                        totalStats.m_myStats.m_winsCount[0] += pairStats.m_myStats.m_winsCount[0];
                        totalStats.m_myStats.m_winsCount[1] += pairStats.m_myStats.m_winsCount[1];
                        totalStats.m_myStats.m_drawsCount += pairStats.m_myStats.m_drawsCount;
                        totalStats.m_oppStats.m_drawsCount += pairStats.m_oppStats.m_drawsCount;
                        totalStats.m_oppStats.m_winsCount[0] += pairStats.m_oppStats.m_winsCount[0];
                        totalStats.m_oppStats.m_winsCount[1] += pairStats.m_oppStats.m_winsCount[1];

                        printf("\n");
                        printf("Opp: %s\n", pairStats.m_oppBotName.c_str());
                        printf("\tWins=%.2f%% (X=%.2f%%; O=%.2f%%)\n", pairStats.GetMyWinsPercentage(), pairStats.GetMyXWinsPercentage(), pairStats.GetMyOWinsPercentage());
                        printf("\tDraws=%.2f%%\n", pairStats.GetDrawPercentage());
                        printf("\tLoses=%.2f%% (X=%.2f%%; O=%.2f%%)\n", pairStats.GetMyLosesPercentage(), pairStats.GetMyXLosesPercentage(), pairStats.GetMyOLosesPercentage());
                    }


                    printf("\n");
                    printf("TOTAL:\n");
                    printf("\tWins=%.2f%% (X=%.2f%%; O=%.2f%%)\n", totalStats.GetMyWinsPercentage(), totalStats.GetMyXWinsPercentage(), totalStats.GetMyOWinsPercentage());
                    printf("\tDraws=%.2f%%\n", totalStats.GetDrawPercentage());
                    printf("\tLoses=%.2f%% (X=%.2f%%; O=%.2f%%)\n", totalStats.GetMyLosesPercentage(), totalStats.GetMyXLosesPercentage(), totalStats.GetMyOLosesPercentage());
                }
            }
        };
    }

    std::vector<IBot*> CreateBots()
    {
        auto randomBotWithOpening = new CCustomBot(
            [](game::SGameState const& gameState, int const myPlayer, bool const debugEnabled)
            {
                if (GAME_STATE_ELEMENTS_COUNT(gameState) == 0) return SVec2{ 4, 4 };

                return CRandomBot_v1::FindTurn(gameState, myPlayer, debugEnabled);
            }, "Random_v2"
        );

        return std::vector<IBot*>{
            randomBotWithOpening
            , new CRandomBot_v1()
            , new CMinimaxBot_v1()
        };
    }

    void FreeBots(std::vector<IBot*>& bots)
    {
        for (auto bot : bots)
        {
            delete bot;
        }
    }

    void Run_Playground_Cases()
    {
        auto bots = CreateBots();
        int const matchesCount = 20;

        auto playground = CConsoleBotsPlayground().RunPlayground(bots, matchesCount);

        FreeBots(bots);
    }
}
}