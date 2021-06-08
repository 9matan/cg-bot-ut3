#pragma once

#include <array>
#include <functional>
#include <string>
#include <vector>

#include "mimax/common/Random.h"

#include "bot-core/bot/IBot.h"
#include "bot-core/ut3-game/GameState.h"
#include "bot-core/ut3-game/GameStateHelper.h"

namespace ut3 {
namespace bot {

    class CPlayground
    {
    public:
        struct SBotPairStats
        {
            SBotPairStats()
                : m_drawsCount(0)
            {
                memset(m_winsCount, 0, sizeof(m_winsCount));
            }

            inline int GetXWins() const { return m_winsCount[0]; }
            inline int GetOWins() const { return m_winsCount[1]; }
            inline int GetTotalWins() const { return GetXWins() + GetOWins(); }

            // first - X or O; second - bot index
            int m_winsCount[2];
            int m_drawsCount;
        };

        struct SBotStats
        {
            struct SPairEntry
            {
                SBotPairStats m_myStats;
                SBotPairStats m_oppStats;
                std::string m_oppBotName;

                inline int GetMatchesCount() const { return m_myStats.GetTotalWins() + m_oppStats.GetTotalWins() + m_myStats.m_drawsCount; }
                inline float GetMyWinsPercentage() const { return 100.0f * (float)(m_myStats.GetTotalWins()) / (float)(GetMatchesCount()); }
                inline float GetMyXWinsPercentage() const { return 100.0f * (float)(m_myStats.GetXWins()) / (float)(GetMatchesCount()); }
                inline float GetMyOWinsPercentage() const { return 100.0f * (float)(m_myStats.GetOWins()) / (float)(GetMatchesCount()); }
                inline float GetDrawPercentage() const { return 100.0f * (float)(m_myStats.m_drawsCount) / (float)(GetMatchesCount());}
                inline float GetMyLosesPercentage() const { return 100.0f * (float)(m_oppStats.GetTotalWins()) / (float)(GetMatchesCount()); }
                inline float GetMyXLosesPercentage() const { return 100.0f * (float)(m_oppStats.GetOWins()) / (float)(GetMatchesCount()); }
                inline float GetMyOLosesPercentage() const { return 100.0f * (float)(m_oppStats.GetXWins()) / (float)(GetMatchesCount()); }
            };

            std::string m_botName;
            std::vector<SPairEntry> m_pairsStats;

            inline void AddPairEntry(SBotPairStats const& myStats, SBotPairStats const& oppStats, std::string const& oppName)
            {
                SPairEntry entry;
                entry.m_myStats = myStats;
                entry.m_oppStats = oppStats;
                entry.m_oppBotName = oppName;

                m_pairsStats.push_back(entry);
            }
        };
        
        struct SMatchStats
        {
            std::string m_botName;
            int m_winnerPlayer;
        };

    public:
        std::array<SBotPairStats, 2> RunPairPlayground(std::array<bot::IBot*, 2> const& bots, int const matchesCount);
        std::vector<SBotStats> RunPlayground(std::vector<bot::IBot*> const& bots, int const matchesCountPerPair);

    protected:
        virtual void OnPlaygroundStarted() {}
        virtual void OnPairPlaygroundStarted(std::array<std::string, 2> const& /*botNames*/) {}
        virtual void OnMatchStart(int const /*matchIndex*/) {}
        virtual void OnMatchEnded(game::SGameState const& /*gameState*/, SMatchStats const& /*stats*/) {}
        virtual void OnPlaygroundEnded(std::vector<SBotStats> const& /*stats*/) {}
    };

}
}