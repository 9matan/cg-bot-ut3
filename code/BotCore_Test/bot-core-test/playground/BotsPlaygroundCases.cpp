#include "bot-core/ut3-game/GameStateView.h"

#include "bot-core/bot/MinimaxBot_v1.h"
#include "bot-core/bot/RandomBot_v1.h"

#include "bot-core-test/playground/BotsPlayground.h"

namespace ut3
{
    void PrintResults(CPlayground::SGlobalStatistics const& stats)
    {
        printf("\n");
        printf("=========================\n");
        printf("== PLAYGROUND RESULTS ===\n");
        printf("=========================\n");

        printf("Bot 0:\n");
        printf("\tWins %i:\n", stats.GetTotalWins(0));
        printf("\t\tWins with X %i:\n", stats.GetXWins(0));
        printf("\t\tWins with O %i:\n", stats.GetOWins(0));
        printf("\tDraws %i:\n", stats.m_drawsCount);

        printf("\n");

        printf("Bot 1:\n");
        printf("\tWins %i:\n", stats.GetTotalWins(1));
        printf("\t\tWins with X %i:\n", stats.GetXWins(1));
        printf("\t\tWins with O %i:\n", stats.GetOWins(1));
        printf("\tDraws %i:\n", stats.m_drawsCount);
    }

    void Run_Playground_Cases()
    {
        printf("=========================\n");
        printf("== PLAYGROUND ===========\n");
        printf("=========================\n");

        CPlayground playground;
        playground.SetOnMatchStart([](int const matchindex)
            {
                printf("Match #%i:", matchindex);
            }
        );
        playground.SetOnMatchEnded([](game::SGameState const& gameState, CPlayground::SMatchStatistics matchStats)
            {
                if (matchStats.m_winner == GAME_STATE_ELEMENT_DRAW)
                {
                    printf(" D\n");
                }
                else
                {
                    printf(" bot_%i (%c)\n", matchStats.m_winnerBotIndex, game::ConvertGameStateElemToChar(matchStats.m_winner));
                }
                game::SGameStateView(gameState).Print();
                printf("\n");
            }
        );
        playground.PlayGames<bot::CMinimaxBot_v1, bot::CRandomBot_v1>(20);

        PrintResults(playground.GetStatistics());
    }
}