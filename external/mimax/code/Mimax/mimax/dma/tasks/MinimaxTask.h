#pragma once

#include <optional>

#include "mimax/mt/Task.h"

namespace mimax {
namespace dma {

template<typename TMinimax>
class CMinimaxTask : public mimax::mt::ITask
{
public:
    using State = typename TMinimax::State;
    using Move = typename TMinimax::Move;

public:
    CMinimaxTask(TMinimax* minimax, State const& state)
        : m_minimax(minimax)
        , m_state(state)
    {}

    void RunTask() override
    {
        m_result = m_minimax->FindSolution(m_state);
    }

    void StopTask() override
    {
        m_minimax->StopAlgorithm();
    }

    inline std::optional<Move> const& GetResult() const { return m_result; }

private:
    State m_state;
    TMinimax* m_minimax;
    std::optional<Move> m_result;
};

} // dma
} // mimax