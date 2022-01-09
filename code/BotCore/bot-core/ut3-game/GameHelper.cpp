#include "BotCore_PCH.h"
#include "bot-core/ut3-game/GameHelper.h"

#include "mimax/common/Random.h"
#include "mimax/common/StdContainerHelper.h"

#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameState.h"

namespace ut3 {
namespace game {

namespace {

char WORLD_POS_TO_LOCAL[9] = { 0, 1, 2, 0, 1, 2, 0, 1, 2 };

} // namespace

SGameState PlayGame(std::array<FindTurnFunc, 2> bots, SGameState gameState)
{
    int currentBotIndex = 0;
    while (GAME_STATE_GET_GAME_WINNER(gameState) == GAME_STATE_ELEMENT_EMPTY)
    {
        SVec2 const turn = bots[currentBotIndex](gameState);

        Turns turns;
        CollectPossibleTurns(gameState, turns);
        assert(std::find(turns.begin(), turns.end(), turn) != turns.end());

        MakeTurn(gameState, turn[0], turn[1]);
        currentBotIndex = ((currentBotIndex + 1) & 1);
    }

    return gameState;
}

SGameState PlayRandomGame(SGameState gameState, size_t const maxDepth)
{
    assert(GAME_STATE_IS_VALID_LAST_OPP_TURN(gameState));

    using BlockTurns = mimax::common::CStaticVector<SVec2, 9>;
    BlockTurns availableTurns[9];
    size_t totalTurnsCount = 0;
    for (size_t blockX = 0; blockX < 3; ++blockX)
    {
        for (size_t blockY = 0; blockY < 3; ++blockY)
        {
            size_t const blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(blockX, blockY);
            if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY)
            {
                auto const block = GetBlockState(gameState, blockIndex);
                for (size_t x = 0; x < 3; ++x)
                {
                    for (size_t y = 0; y < 3; ++y)
                    {
                        if (GAME_BLOCK_STATE_GET_ELEMENT_BY_POS(block, x, y) == GAME_STATE_ELEMENT_EMPTY)
                        {
                            availableTurns[blockIndex].push_back({ x + 3 * blockX, y + 3 * blockY });
                            ++totalTurnsCount;
                        }
                    }
                }
            }
        }
    }

    SVec2 lastTurn(GAME_STATE_GET_LAST_OPP_TURN_X(gameState), GAME_STATE_GET_LAST_OPP_TURN_Y(gameState));
    SVec2 turn;
    for (size_t i = 0; i < maxDepth && GAME_STATE_GET_GAME_WINNER(gameState) == GAME_STATE_ELEMENT_EMPTY; ++i)
    {
        size_t blockIndex = GAME_BLOCK_STATE_POS_TO_INDEX(WORLD_POS_TO_LOCAL[lastTurn[0]], WORLD_POS_TO_LOCAL[lastTurn[1]]);
        size_t turnIndex;
        if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) == GAME_STATE_ELEMENT_EMPTY)
        {
            turnIndex = mimax::common::GetRandomSizeT(0, availableTurns[blockIndex].size());
        }
        else
        {
            turnIndex = mimax::common::GetRandomSizeT(0, totalTurnsCount);
            blockIndex = 0;
            while (turnIndex >= availableTurns[blockIndex].size())
            {
                turnIndex -= availableTurns[blockIndex].size();
                ++blockIndex;
            }
        }

        turn = availableTurns[blockIndex][turnIndex];
        std::swap(availableTurns[blockIndex][turnIndex], availableTurns[blockIndex].back());
        availableTurns[blockIndex].pop_back();
        --totalTurnsCount;

        MakeTurn(gameState, turn[0], turn[1]);
        lastTurn = turn;
        if (GAME_STATE_GET_BLOCK_STATUS(gameState, blockIndex) != GAME_STATE_ELEMENT_EMPTY)
        {
            totalTurnsCount -= availableTurns[blockIndex].size();
            availableTurns[blockIndex].clear();
        }
    }

    return gameState;
}

}
}