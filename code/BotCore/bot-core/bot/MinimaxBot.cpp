#include "BotCore_PCH.h"
#include "bot-core/bot/MinimaxBot.h"

#include <bitset>
#include <chrono>
#include <thread>

#include "mimax/minimax/MinimaxAlgorithmBase.h"
#include "bot-core/ut3-game/Game.h"

using namespace std::chrono_literals;

namespace ut3
{
    namespace
    {
        constexpr unsigned int PLAYERS_GLOBAL_BLOCK_MASKS[2] = {
            87381,
            174762
        };

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
                    if (gameWinner == GAME_STATE_PLAYER_INDEX_TO_PLAYER_ELEMENT(m_myPlayer)) return 10.0f;
                    return -10.0f;
                }

                auto const globalBlock = GAME_STATE_GET_GLOBAL_BLOCK(state);

                int const oppPlayer = (m_myPlayer & 1);
                
                size_t const myBlocksCnt = std::bitset<32>(globalBlock.m_data & PLAYERS_GLOBAL_BLOCK_MASKS[m_myPlayer]).count();
                size_t const oppBlocksCnt = std::bitset<32>(globalBlock.m_data & PLAYERS_GLOBAL_BLOCK_MASKS[oppPlayer]).count();

                return (float)(myBlocksCnt - oppBlocksCnt);
            }

            void GetPossibleMoves(game::Turns& turnsOut, game::SGameState const& state)
            {
                game::CollectPossibleTurns(state, turnsOut);
            }

            void MakeMove(game::SGameState& state, SVec2 move)
            {
                game::MakeTurn(state, move[0], move[1]);
            }

        private:
            int m_myPlayer;
        };
    }

    CMinimaxBot::CMinimaxBot(SInputData /*initData*/)
        : m_myPlayer(-1)
    {
    }

    SOutputData CMinimaxBot::FirstUpdate(SInputData initData)
    {
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

    SOutputData CMinimaxBot::Update(SInputData turnData)
    {
        game::MakeTurn(m_gameState, turnData.m_oppTurnX, turnData.m_oppTurnY);

        using CUT3MinimaxAlgo = mimax::minimax::CMinimaxAlgorithmBase<game::SGameState, SVec2, game::Turns, CUT3MinimaxResolver>;

        CUT3MinimaxAlgo minimax(5, CUT3MinimaxResolver(m_myPlayer));
        SVec2 turn1 = minimax.Solve(m_gameState);

        game::MakeTurn(m_gameState, turn1[0], turn1[1]);
        return { turn1[0], turn1[1] };
    }
}