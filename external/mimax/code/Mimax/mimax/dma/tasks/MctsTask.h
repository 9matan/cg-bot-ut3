#pragma once

#include "mimax/mt/Task.h"

namespace mimax {
namespace dma {

template<typename TMcts>
class CMctsTask : public mimax::mt::ITask
{
public:
    CMctsTask(TMcts* mcts)
        : m_mcts(mcts)
        , m_isStopRequested(false)
    {}

    void RunTask() override
    {
        while (!m_isStopRequested)
        {
            m_mcts->Evaluate();
        }
    }

    void StopTask() override
    {
        m_isStopRequested = true;
    }

private:
    bool m_isStopRequested;
    TMcts* m_mcts;
};

} // dma
} // mimax