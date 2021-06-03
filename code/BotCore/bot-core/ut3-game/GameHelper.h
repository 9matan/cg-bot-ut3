#pragma once

#include <functional>

#include "mimax/common/Vec2.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace game {

    using FindTurnFunc = std::function<SVec2(SGameState const& gameState)>;

    SGameState PlayGame(std::array<FindTurnFunc, 2> bots, SGameState gameState = SGameState());

}
}