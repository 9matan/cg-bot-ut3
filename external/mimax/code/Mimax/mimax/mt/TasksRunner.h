#pragma once

#include <future>
#include <vector>

namespace mimax {
namespace mt {

class ITask;

class CTasksRunner
{
public:
    void RunTasksAndWait(std::vector<ITask*> const& tasks, std::chrono::microseconds const waitingTime);

private:
    std::vector<ITask*> m_tasks;
    std::vector<std::future<void>> m_futures;

private:
    void RunTasks();
    void Wait(std::chrono::microseconds const time);
    void StopTasks();
    void WaitForTasksCompleted();
};

} //mt
} // mimax