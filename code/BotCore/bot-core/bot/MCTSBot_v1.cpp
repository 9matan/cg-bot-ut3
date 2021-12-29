#include "BotCore_PCH.h"
#include "bot-core/bot/MCTSBot_v1.h"

#include <chrono>
#include <thread>

#include "mimax/common/Random.h"
#include "mimax/dma/MctsBase.h"
#include "mimax/dma/MctsUtility.h"
#include "mimax/dma/tasks/MctsTask.h"
#include "mimax/mt/TasksRunner.h"

#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameState.h"
#include "bot-core/ut3-game/GameStateView.h"
//#include "bot-core/bot/MinimaxBot_v1.h"

namespace ut3 {
namespace bot {

namespace {

class CUT3MctsResolver
{
public:
    inline static float CalculateScore(game::SGameState const& state, int myPlayer)
    {
        int const gameWinner = GAME_STATE_GET_GAME_WINNER(state);
        if (gameWinner != GAME_STATE_ELEMENT_EMPTY)
        {
            if (gameWinner == GAME_STATE_ELEMENT_DRAW) return 0.5f;
            else if (gameWinner == GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(myPlayer)) return 1.0f;
            else return 0.0f;
        }

        auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);
        int const oppPlayer = ((myPlayer + 1) & 1);
        int const myBlocksCnt = (int)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, myPlayer);
        int const oppBlocksCnt = (int)GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, oppPlayer);
        return (myBlocksCnt == oppBlocksCnt
            ? 0.5f
            : (myBlocksCnt > oppBlocksCnt ? 1.0f : 0.0f));
    }

public:
    void GetPossibleMoves(game::SGameState const& state, game::Turns& movesOut)
    {
        game::CollectPossibleTurns(state, movesOut);
    }

    void MakeMove(game::SGameState& state, SVec2 const move)
    {
        game::MakeTurn(state, move[0], move[1]);
    }

    float Playout(game::SGameState const& state)
    {
        int myPlayer = GAME_STATE_GET_PLAYER(state);
        game::SGameState curState = state;

        game::Turns moves;
        for (size_t depth = 0; depth < 8; ++depth)
        {
            game::CollectPossibleTurns(curState, moves);
            if (moves.empty()) break;

            auto const move = *mimax::common::GetRandomItem(moves.begin(), moves.end());
            game::MakeTurn(curState, move[0], move[1]);
        }

        return CalculateScore(curState, myPlayer);
    }
};

using CUT3Mcts = mimax::dma::CMctsBase<game::SGameState, SVec2, game::Turns, CUT3MctsResolver>;
using CUT3MctsTask = mimax::dma::CMctsTask<CUT3Mcts>;

static SVec2 CalculateBestMove(std::vector<CUT3Mcts> const& mctsAlgos)
{
    mimax::dma::SMctsNodeEvalResult<SVec2> combinedResult;
    for (auto const& algo : mctsAlgos)
    {
        mimax::dma::mcts_utility::CombineEvalResults(combinedResult, algo.GetRootEvalResult());
    }

    return mimax::dma::mcts_utility::GetBestMove(combinedResult);
}

} // namespace

SVec2 CMCTSBot_v1::FindTurn(game::SGameState const& gameState)
{
    return FindTurn(gameState, m_myPlayer, m_isDebugEnabled);
}

SVec2 CMCTSBot_v1::FindTurn(game::SGameState const& gameState, int const /*myPlayer*/, bool const debugEnabled)
{
    if (GAME_STATE_ELEMENTS_COUNT(gameState) == 0)
    {
        return { 4, 4 };
    }

    std::vector<CUT3Mcts> mctsAlgos;
    std::vector<CUT3MctsTask> mctsTasks;
    std::vector<mimax::mt::ITask*> tasksToRun;

    auto const hardwareConcurrency = std::thread::hardware_concurrency();
    mctsAlgos.reserve(hardwareConcurrency);
    mctsTasks.reserve(hardwareConcurrency);
    tasksToRun.reserve(hardwareConcurrency);

    std::mt19937_64 randEngine(time(NULL));
    CUT3MctsResolver resolver;
    for (size_t i = 0; i < hardwareConcurrency; ++i)
    {
        auto& mcts = mctsAlgos.emplace_back(gameState, resolver, randEngine(), 1.41f);
        auto& mctsTask = mctsTasks.emplace_back(&mcts);
        tasksToRun.push_back(&mctsTask);
    }

    mimax::mt::CTasksRunner().RunTasksAndWait(tasksToRun, std::chrono::milliseconds(90));

    if (debugEnabled)
    {
#if MIMAX_MCTS_DEBUG
        std::cerr << mctsAlgos.front().GetDebugInfo();
#endif // MIMAX_MCTS_DEBUG
    }

    return CalculateBestMove(mctsAlgos);
}

}
}