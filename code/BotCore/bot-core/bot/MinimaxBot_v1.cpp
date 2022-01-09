#include "BotCore_PCH.h"
#include "bot-core/bot/MinimaxBot_v1.h"

#include <chrono>
#include <thread>

#include "mimax/common/Random.h"
#include "mimax/common/Vec2Helper.h"
#include "mimax/dma/MinimaxBase.h"
#include "mimax/dma/tasks/MinimaxTask.h"
#include "mimax/mt/TasksRunner.h"

#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameStateView.h"

using namespace std::chrono_literals;

namespace ut3 {
namespace bot {

namespace
{
    class CUT3MinimaxResolver
    {
    public:
        CUT3MinimaxResolver(int const myPlayer)
            : m_myPlayer(myPlayer)
        {}

        float EvaluateState(game::SGameState const& state)
        {
            int const gameWinner = GAME_STATE_GET_GAME_WINNER(state);
            if (gameWinner != GAME_STATE_ELEMENT_EMPTY)
            {
                if (gameWinner == GAME_STATE_ELEMENT_DRAW) return 0.0f;
                float const depthScore = GAME_STATE_ELEMENTS_COUNT(state) * 0.01f;
                if (gameWinner == GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(m_myPlayer)) return 10.0f - depthScore;
                return -(10.0f - depthScore);
            }

            auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);

            int const oppPlayer = ((m_myPlayer + 1) & 1);
                
            int const myBlocksCnt = (int)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, m_myPlayer);
            int const oppBlocksCnt = (int)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, oppPlayer);

            return (float)(myBlocksCnt - oppBlocksCnt);
        }

        void GetPossibleMoves(game::Turns& turnsOut, game::SGameState const& state)
        {
            game::CollectPossibleTurns(state, turnsOut);
            mimax::common::RandomShuffle(turnsOut.begin(), turnsOut.end());
        }

        void MakeMove(game::SGameState& state, SVec2 move)
        {
            game::MakeTurn(state, move[0], move[1]);
        }

    private:
        int m_myPlayer;
    };

    using CUT3MinimaxAlgo = mimax::dma::CMinimaxBase<game::SGameState, SVec2, game::Turns, CUT3MinimaxResolver>;
    using CMinimaxTask = mimax::dma::CMinimaxTask<CUT3MinimaxAlgo>;
}

CMinimaxBot_v1::CMinimaxBot_v1()
    : CBotBase("CMinimaxBot_v1")
{}

SVec2 CMinimaxBot_v1::FindTurn(game::SGameState const& gameState)
{
    return FindTurn(gameState, m_myPlayer, m_isDebugEnabled);
}

SVec2 CMinimaxBot_v1::FindTurn(game::SGameState const& gameState, int const myPlayer, bool const debugEnabled)
{
    if (GAME_STATE_ELEMENTS_COUNT(gameState) == 0)
    {
        return { 4, 4 };
    }

    auto globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(gameState);
    size_t const playedBlocksCount =
        GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 0)
        + GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 1)
        + GAME_STATE_BLOCK_COUNT_DRAW_ELEMENTS(globalBlock);
    float const minScoreValue = (playedBlocksCount >= 6 ? (-1.0f) : (-2.0f));

    std::vector<CUT3MinimaxAlgo> minimaxAlgos;
    std::vector<CMinimaxTask> minimaxTasks;
    std::vector<mimax::mt::ITask*> tasksToRun;
    CUT3MinimaxResolver resolver(myPlayer);
    CUT3MinimaxAlgo::SConfig config;
    config.m_minValue = minScoreValue;
    config.m_maxValue = 10.0f;
    config.m_epsilon = 0.001f;

    auto const hardwareConcurrency = std::thread::hardware_concurrency();
    minimaxAlgos.reserve(hardwareConcurrency);
    minimaxTasks.reserve(hardwareConcurrency);
    tasksToRun.reserve(hardwareConcurrency);
    size_t const step = (hardwareConcurrency <= 4) ? 2 : 1;

    size_t constexpr minDepth = 6;
    for (size_t depth = minDepth; depth < minDepth + step * hardwareConcurrency; depth += step)
    {
        config.m_maxDepth = depth;
        auto& minimaxAlgo = minimaxAlgos.emplace_back(resolver, config);
        auto& minimaxTask = minimaxTasks.emplace_back(&minimaxAlgo, gameState);
        tasksToRun.push_back(&minimaxTask);
    }

    mimax::mt::CTasksRunner().RunTasksAndWait(tasksToRun, 95ms);
    SVec2 turn = { -1, -1 };
    for (int i = (int)minimaxTasks.size() - 1; i >= 0; --i)
    {
        auto& move = minimaxTasks[i].GetResult();
        if (move.has_value())
        {
            turn = move.value();
#if MIMAX_MINIMAX_DEBUG
            if (debugEnabled)
            {
                std::cerr << minimaxAlgos[i].GetDebugInfo();
            }
#endif // MIMAX_MINIMAX_DEBUG
            break;
        }
    }

    if (turn[0] == -1)
    {
        std::cerr << "[ERROR] Could not find a next turn using minimax!\n";
        game::Turns turns;
        game::CollectPossibleTurns(gameState, turns);
        turn = *mimax::common::GetRandomItem(turns.begin(), turns.end());
    }

    return turn;
}

}
}