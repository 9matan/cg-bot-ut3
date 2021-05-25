#include "mimax_test/games/TicTacToeGame.h"

#include <cassert>

namespace mimax_test {
namespace games {
namespace tic_tac_toe {

    bool SGameState::operator==(SGameState const& another) const
    {
        if (m_player != another.m_player) return false;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (m_map[i][j] != another.m_map[i][j])
                    return false;
            }
        }
        return true;
    }

    void MakeMove(SGameState& state, SMove const move)
    {
        state.m_map[move.first][move.second] = state.m_player;
        state.m_player = state.m_player == 'X' ? 'O' : 'X';
    }

    char GetWinner(SGameState const& state)
    {
        if (state.m_map[0][0] == state.m_map[0][1] && state.m_map[0][1] == state.m_map[0][2])      return state.m_map[0][0];
        else if (state.m_map[1][0] == state.m_map[1][1] && state.m_map[1][1] == state.m_map[1][2]) return state.m_map[1][0];
        else if (state.m_map[2][0] == state.m_map[2][1] && state.m_map[2][1] == state.m_map[2][2]) return state.m_map[2][0];
        else if (state.m_map[0][0] == state.m_map[1][0] && state.m_map[1][0] == state.m_map[2][0]) return state.m_map[0][0];
        else if (state.m_map[0][1] == state.m_map[1][1] && state.m_map[1][1] == state.m_map[2][1]) return state.m_map[0][1];
        else if (state.m_map[0][2] == state.m_map[1][2] && state.m_map[1][2] == state.m_map[2][2]) return state.m_map[0][2];
        else if (state.m_map[0][0] == state.m_map[1][1] && state.m_map[1][1] == state.m_map[2][2]) return state.m_map[0][0];
        else if (state.m_map[2][0] == state.m_map[1][1] && state.m_map[1][1] == state.m_map[0][2]) return state.m_map[2][0];

        int emptyCnt = 0;
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (state.m_map[i][j] == '-')
                    ++emptyCnt;
            }
        }

        return emptyCnt > 0 ? '-' : 'D';
    }

    char PlayGame(SGameState state, FindNextMoveFunc nextMoveFunc)
    {
        for (int i = 0; i <= 9; ++i)
        {
            char const winner = GetWinner(state);
            if (winner != '-') return winner;

            auto const nextMove = nextMoveFunc(state);
            MakeMove(state, nextMove);
        }
        assert(false);
        return '-';
    }
}
}
}