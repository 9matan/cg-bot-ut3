#pragma once

#include "mimax/common/Vec2.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace game {

    char ConvertGameStateElemToChar(int const elem);
    int ConvertCharToGameStateElem(char const c);

    SGameState CreateGameState(char const map[9][10], SVec2 const oppLastTurn);

} // game
} // ut3