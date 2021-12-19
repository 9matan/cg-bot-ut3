#include "Mimax_PCH.h"
#include "mimax/mt/TasksRunner.h"

#include "mimax/mt/Task.h"

namespace mimax {
namespace mt {

using namespace std;

void CTasksRunner::RunTasksAndWait(vector<ITask*> const& tasks, chrono::microseconds const waitingTime)
{
    m_tasks = tasks;
    m_futures.clear();

    RunTasks();
    Wait(waitingTime);
    StopTasks();
    WaitForTasksCompleted();
}

void CTasksRunner::RunTasks()
{
    for (auto task : m_tasks)
    {
        auto future = async([task]()
            {
                task->RunTask();
            });
        m_futures.emplace_back(move(future));
    }
}

void CTasksRunner::Wait(chrono::microseconds const time)
{
    auto const startTime = chrono::high_resolution_clock::now();
    auto const endTime = startTime + time;

    while (chrono::high_resolution_clock::now() < endTime)
    {
        this_thread::yield();
    }
}

void CTasksRunner::StopTasks()
{
    for (auto task : m_tasks)
    {
        task->StopTask();
    }
}

void CTasksRunner::WaitForTasksCompleted()
{
    for (auto& future : m_futures)
    {
        future.wait();
    }
}

} // mt
} // mimax