#include "BotCore_PCH.h"
#include "bot-core/bot/MCTSBot_v1.h"

#include <chrono>
#include <thread>

#include "mimax/common/Random.h"
#include "mimax/dma/MctsUtility.h"
#include "mimax/dma/tasks/MctsTask.h"
#include "mimax/mt/TasksRunner.h"

#include "bot-core/ut3-game/GameStateView.h"

namespace ut3 {
namespace bot {

using CUT3MctsTask = mimax::dma::CMctsTask<CUT3Mcts>;

namespace {

inline static SVec2 CalculateBestMove(std::vector<CUT3Mcts> const& mctsAlgos)
{
    mimax::dma::SMctsNodeEvalResult<SVec2> combinedResult;
    for (auto const& algo : mctsAlgos)
    {
        mimax::dma::mcts_utility::CombineEvalResults(combinedResult, algo.GetRootEvalResult());
    }

    return mimax::dma::mcts_utility::GetBestMove(combinedResult);
}

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

#if MIMAX_MCTS_DEBUG
inline static CMCTSBot_v1::SDebugStats CalculateDebugStats(std::vector<CUT3Mcts> const& mctsAlgos)
{
    CMCTSBot_v1::SDebugStats debugStats;

    for (auto const& algo : mctsAlgos)
    {
        auto const& debugInfo = algo.GetDebugInfo();
        debugStats.m_simulationsCnt += debugInfo.m_simulationsCnt;
        debugStats.m_totalNodesCnt += debugInfo.m_totalNodesCnt;
        debugStats.m_totalVisitedNodesCnt += debugInfo.m_totalVisitedNodesCnt;
    }

    debugStats.m_simulationsCnt /= (float)mctsAlgos.size();
    debugStats.m_totalNodesCnt /= (float)mctsAlgos.size();
    debugStats.m_totalVisitedNodesCnt /= (float)mctsAlgos.size();

    return debugStats;
}

inline static CMCTSBot_v1::SDebugStats GetAverageDebugStats(std::vector<CMCTSBot_v1::SDebugStats> const& debugStatsHistory)
{
    CMCTSBot_v1::SDebugStats averageDebugStats;

    for (auto const& debugStats : debugStatsHistory)
    {
        averageDebugStats.m_simulationsCnt += debugStats.m_simulationsCnt;
        averageDebugStats.m_totalNodesCnt += debugStats.m_totalNodesCnt;
        averageDebugStats.m_totalVisitedNodesCnt += debugStats.m_totalVisitedNodesCnt;

    }

    averageDebugStats.m_simulationsCnt /= (float)debugStatsHistory.size();
    averageDebugStats.m_totalNodesCnt /= (float)debugStatsHistory.size();
    averageDebugStats.m_totalVisitedNodesCnt /= (float)debugStatsHistory.size();

    return averageDebugStats;

}
#endif // MIMAX_MCTS_DEBUG

} // namespace

template<size_t TCacheSize>
void CUT3MctsCachedMathHelper<TCacheSize>::Initialize()
{
    static bool isInitialized = false;
    if (isInitialized) return;
    isInitialized = true;

    ms_sqrtCache[0] = 0.0f;
    for (int i = 1; i < TCacheSize; ++i)
    {
        ms_sqrtCache[i] = ::sqrt((float)i / 100.0f);
        ms_logCache[i] = (float)::log(i);
    }
}

template<size_t TCacheSize>
std::array<float, TCacheSize> CUT3MctsCachedMathHelper<TCacheSize>::ms_sqrtCache;

template<size_t TCacheSize>
std::array<float, TCacheSize> CUT3MctsCachedMathHelper<TCacheSize>::ms_logCache;

CUT3MctsResolver::CUT3MctsResolver(size_t const maxDepth)
    : m_maxDepth(maxDepth)
{
}

float CUT3MctsResolver::Playout(game::SGameState const& state)
{
    int myPlayer = GAME_STATE_GET_PLAYER(state);
    game::SGameState curState = state;

    game::Turns moves;
    for (size_t depth = 0; depth <= m_maxDepth; ++depth)
    {
        game::CollectPossibleTurns(curState, moves);
        if (moves.empty()) break;

        auto const move = *mimax::common::GetRandomItem(moves.begin(), moves.end());
        game::MakeTurn(curState, move[0], move[1]);
    }

    return CalculateScore(curState, myPlayer);
}

CMCTSBot_v1::CMCTSBot_v1()
    : CMCTSBot_v1(1.41f, 7)
{}

CMCTSBot_v1::CMCTSBot_v1(float explorationParam, size_t const maxDepth)
{
    CUT3MctsMathHelper::Initialize();

    auto const hardwareConcurrency = std::thread::hardware_concurrency();
    m_mctsAlgos.reserve(hardwareConcurrency);

    std::mt19937_64 randEngine(time(NULL));
    CUT3MctsResolver resolver(maxDepth);
    CUT3Mcts::SConfig config;
    config.m_nodesPoolSize = 200000;
    config.m_explorationParam = explorationParam;
    for (size_t i = 0; i < hardwareConcurrency; ++i)
    {
        config.m_randomSeed = randEngine();
        m_mctsAlgos.emplace_back(resolver, config);
    }
}

void CMCTSBot_v1::Reset()
{
#if MIMAX_MCTS_DEBUG
    m_debugStatsHistory.clear();
#endif // MIMAX_MCTS_DEBUG
    CBotBase::Reset();
}

void CMCTSBot_v1::OnMatchEnded()
{
#if MIMAX_MCTS_DEBUG
    auto const debugStats = GetAverageDebugStats(m_debugStatsHistory);

    std::cerr << "\n";
    std::cerr << "Simulations count: " << debugStats.m_simulationsCnt << "\n";
    std::cerr << "Nodes count: " << debugStats.m_totalNodesCnt << "\n";
    std::cerr << "Visited nodes count: " << debugStats.m_totalVisitedNodesCnt << "\n";
#endif // MIMAX_MCTS_DEBUG
}

SVec2 CMCTSBot_v1::FindTurn(game::SGameState const& gameState)
{
    if (GAME_STATE_ELEMENTS_COUNT(gameState) == 0)
    {
        return { 4, 4 };
    }

    std::vector<CUT3MctsTask> mctsTasks;
    std::vector<mimax::mt::ITask*> tasksToRun;

    mctsTasks.reserve(m_mctsAlgos.size());
    tasksToRun.reserve(m_mctsAlgos.size());

    for (size_t i = 0; i < m_mctsAlgos.size(); ++i)
    {
        m_mctsAlgos[i].Reset(gameState);
        auto& mctsTask = mctsTasks.emplace_back(&(m_mctsAlgos[i]));
        tasksToRun.push_back(&mctsTask);
    }

    mimax::mt::CTasksRunner().RunTasksAndWait(tasksToRun, std::chrono::milliseconds(90));

    if (m_isDebugEnabled)
    {
#if MIMAX_MCTS_DEBUG
        std::cerr << m_mctsAlgos.front().GetDebugInfo();
#endif // MIMAX_MCTS_DEBUG
    }

#if MIMAX_MCTS_DEBUG
    m_debugStatsHistory.push_back(CalculateDebugStats(m_mctsAlgos));
#endif // MIMAX_MCTS_DEBUG

    return CalculateBestMove(m_mctsAlgos);
}

}
}