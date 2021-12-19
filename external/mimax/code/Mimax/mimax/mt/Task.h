#pragma once

namespace mimax {
namespace mt {

class ITask
{
public:
    virtual ~ITask() = default;

    virtual void RunTask() = 0;
    virtual void StopTask() = 0;
};

} //mt
} // mimax