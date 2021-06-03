#pragma once

#include <array>
#include <functional>

#include "mimax/common/Random.h"

#include "bot-core/bot/IBot.h"
#include "bot-core/ut3-game/GameState.h"
#include "bot-core/ut3-game/GameStateHelper.h"

namespace ut3
{
    game::SGameState PlayGame(std::array<IBot*, 2> bots);

    class CPlayground
    {
    public:
        struct SGlobalStatistics
        {
            SGlobalStatistics()
                : m_drawsCount(0)
            {
                memset(m_winsCount, 0, sizeof(m_winsCount));
            }

            inline int GetXWins(int const botIndex) const { return m_winsCount[0][botIndex]; }
            inline int GetOWins(int const botIndex) const { return m_winsCount[1][botIndex]; }
            inline int GetTotalWins(int const botIndex) const { return GetXWins(botIndex) + GetOWins(botIndex);  }

            // first - X or O; second - bot index
            int m_winsCount[2][2];
            int m_drawsCount;
        };

        struct SMatchStatistics
        {
            int m_winnerBotIndex;
            int m_winner;
        };

    public:
        using OnMatchStartFunc = std::function<void(int matchIndex)>;
        using OnMatchEndedFunc = std::function<void(game::SGameState const& gameState, SMatchStatistics matchStats)>;

    public:
        template<typename TBot1, typename TBot2>
        void PlayGames(int const matchesCount)
        {
            mimax::common::UpdateRandomSeed();
            for (int matchIndex = 0; matchIndex < matchesCount * 2; ++matchIndex)
            {
                if (m_onMatchStart) m_onMatchStart(matchIndex);

                TBot1 bot0;
                TBot2 bot1;

                auto const gameState = (matchIndex & 1)
                    ? PlayGame({ &bot1, &bot0 })
                    : PlayGame({ &bot0, &bot1 });
                int const xPlayerBotIndex = (matchIndex & 1) ? 1 : 0;
                
                auto const matchStats = UpdateStats(gameState, xPlayerBotIndex);
                if (m_onMatchEnded) m_onMatchEnded(gameState, matchStats);
            }
        }

        inline void SetOnMatchStart(OnMatchStartFunc onMatchStart) { m_onMatchStart = onMatchStart; }
        inline void SetOnMatchEnded(OnMatchEndedFunc onMatchEnded) { m_onMatchEnded = onMatchEnded; }
        
        inline SGlobalStatistics const& GetStatistics() const { return m_stats; }

    private:
        SGlobalStatistics m_stats;
        OnMatchStartFunc m_onMatchStart = nullptr;
        OnMatchEndedFunc m_onMatchEnded = nullptr;

    private:
        SMatchStatistics UpdateStats(game::SGameState const& gameState, int const xPlayerBotIndex);
    };
}