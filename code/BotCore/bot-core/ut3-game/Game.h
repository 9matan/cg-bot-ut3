#pragma once

#include "mimax/common/StaticVector.h"
#include "mimax/common/Vec2.h"

#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace game {

    using Turns = mimax::common::CStaticVector<SVec2, 9 * 9>;

    int CalculateWinner(SGameState const& gameState);
    int CalculateWinner(SGameBlockState const blockState);
    void MakeTurn(SGameState& gameState, int const worldX, int const worldY);
    void CollectPossibleTurns(SGameState const& gameState, Turns& turnsOut);

}
}