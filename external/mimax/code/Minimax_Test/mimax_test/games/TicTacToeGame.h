#pragma once

#include <vector>
#include <functional>

namespace mimax_test {
namespace games {
namespace tic_tac_toe {

    using SMove = std::pair<int, int>;
    struct SGameState
    {
        // '-' empty, 'X' x, 'O' o, 'D' draw
        char m_map[3][4];
        char m_player;

        bool operator==(SGameState const& another) const;
        bool operator!=(SGameState const& another) const { return !this->operator==(another); }
    };

    void MakeMove(SGameState& state, SMove const move);
    char GetWinner(SGameState const& state);

    template<typename TMovesContainer>
    void GetPossibleMoves(TMovesContainer& moves, SGameState const& state)
    {
        moves.clear();
        if (GetWinner(state) != '-') return;

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (state.m_map[i][j] == '-')
                    moves.push_back(std::make_pair(i, j));
            }
        }
    }

    using FindNextMoveFunc = std::function<SMove(SGameState const&)>;
    char PlayGame(SGameState state, FindNextMoveFunc nextMoveFunc);
}
}
}