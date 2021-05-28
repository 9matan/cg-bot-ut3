#pragma once

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace game {

    struct SGameStateView
    {
        SGameStateView(SGameState const& state);
        void Print();

        char m_map[9][10];
        char m_globalBlockStatuses[3][4];
        char m_player;
        char m_oppLastTurnX;
        char m_oppLastTurnY;
        char m_gameWinner;
        unsigned char m_xBlocksCnt;
        unsigned char m_oBlocksCnt;
    };

}
}