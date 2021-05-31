#include "BotCore_PCH.h"
#include "bot-core/bot/MinimaxBot_v1.h"

#include <chrono>
#include <thread>

#include "mimax/common/Random.h"
#include "mimax/common/Vec2Helper.h"
#include "mimax/minimax/MinimaxAlgorithmBase.h"

#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameStateView.h"

using namespace std::chrono_literals;

namespace ut3
{
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

        using CUT3MinimaxAlgo = mimax::minimax::CMinimaxAlgorithmBase<game::SGameState, SVec2, game::Turns, CUT3MinimaxResolver>;

        class CMinimaxThread
        {
        public:
            CMinimaxThread(game::SGameState const& gameState, unsigned int const depth, int const myPlayer, float const minValue)
                : m_minimaxAlgo(depth, CUT3MinimaxResolver(myPlayer), minValue, 10.0f)
                , m_isFinished(false)
                , m_turn(-1, -1)
                , m_thread([this, gameState]()
                    {
                        SVec2 foundTurn = m_minimaxAlgo.Solve(gameState).m_move;
                        if (!m_minimaxAlgo.IsInterrupted())
                            m_turn = foundTurn;
                        m_isFinished = true;
                    }
                )
            {
            }
            
            inline void Stop() { if(!m_isFinished) m_minimaxAlgo.Interrupt(); }
            inline void Join() { m_thread.join(); }
            inline SVec2 GetTurn() const { return m_turn; }
#if MIMAX_MINIMAX_DEBUG
            inline CUT3MinimaxAlgo::SDebugInfo const& GetDebugInfo() const { return m_minimaxAlgo.GetDebugInfo(); }
#endif // MIMAX_MINIMAX_DEBUG

        private:
            std::thread m_thread;
            CUT3MinimaxAlgo m_minimaxAlgo;
            SVec2 m_turn;
            bool m_isFinished;
        };
    }


    CMinimaxBot_v1::CMinimaxBot_v1(SInputData /*initData*/)
        : m_myPlayer(-1)
    {
    }

    SOutputData CMinimaxBot_v1::FirstUpdate(SInputData initData)
    {
        int const randomSeed = mimax::common::UpdateRandomSeed();
        std::cerr << "Random seed: " << randomSeed << "\n";
        std::cerr << "Hardware concurrency: " << std::thread::hardware_concurrency() << "\n";

        if (initData.m_oppTurnX >= 0)
        {
            m_myPlayer = 1;
            return Update(initData);
        }
        else
        {
            m_myPlayer = 0;
            SOutputData output{ 4, 4 };
            game::MakeTurn(m_gameState, output.m_turnX, output.m_turnY);
            return output;
        }
    }

    SOutputData CMinimaxBot_v1::Update(SInputData turnData)
    {
        game::MakeTurn(m_gameState, turnData.m_oppTurnX, turnData.m_oppTurnY);
        game::SGameStateView(m_gameState).Print();

        auto const turn = FindTurn(m_gameState, m_myPlayer);

        game::MakeTurn(m_gameState, turn[0], turn[1]);
        return { turn[0], turn[1] };
    }

    SVec2 CMinimaxBot_v1::FindTurn(game::SGameState const& gameState, int const myPlayer)
    {
        auto const startTime = std::chrono::high_resolution_clock::now();
        auto const endTime = startTime + 95ms;

        auto globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(gameState);
        size_t const playedBlocksCount =
            GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 0)
            + GAME_STATE_BLOCK_COUNT_PLAYER_ELEMENTS(globalBlock, 1)
            + GAME_STATE_BLOCK_COUNT_DRAW_ELEMENTS(globalBlock);
        float const minScoreValue = (playedBlocksCount >= 6 ? (-1.0f) : (-2.0f));

        int const gameStage = (GAME_STATE_ELEMENTS_COUNT(gameState) >= 40) ? 1 : 0;
        int const hardwareConcurrency = std::thread::hardware_concurrency();
        int constexpr threadsCountMax =  6;
        int const threadsCount = (hardwareConcurrency >= 8) ? threadsCountMax : 4;
        CMinimaxThread threads[threadsCountMax] = {
            CMinimaxThread(gameState, 6, myPlayer, minScoreValue)
            , CMinimaxThread(gameState, 7, myPlayer, minScoreValue)
            , CMinimaxThread(gameState, 8, myPlayer, minScoreValue)
            , CMinimaxThread(gameState, 9, myPlayer, minScoreValue)
            , CMinimaxThread(gameState, (hardwareConcurrency >= 8) ? (10 + gameStage) : 1, myPlayer, minScoreValue)
            , CMinimaxThread(gameState, (hardwareConcurrency >= 8) ? (11 + 2 * gameStage) : 1, myPlayer, minScoreValue)
        };

        while (std::chrono::high_resolution_clock::now() < endTime)
        {
            std::this_thread::yield();
        }
        std::cerr << "Stopping threads ... \n";

        for (auto& thread : threads)
        {
            thread.Stop();
        }

        for (auto& thread : threads)
        {
            thread.Join();
        }

        SVec2 turn = { -1, -1 };
        for (int i = threadsCount - 1; i >= 0; --i)
        {
            if (threads[i].GetTurn()[0] != -1)
            {
                turn = threads[i].GetTurn();
#if MIMAX_MINIMAX_DEBUG
                threads[i].GetDebugInfo().Print();
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