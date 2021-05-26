#pragma once

#include <chrono>
#include <ctime>
#include <string>

namespace mimax {
namespace common {

class CTimeProfiler
{
public:
    CTimeProfiler(char const* const name);
    ~CTimeProfiler();
private:
    std::string m_name;
    decltype(std::chrono::high_resolution_clock::now()) m_startTime;
};

}
}

#ifndef DISABLE_PROFILER
#define PROFILE_TIME(name) mimax::common::CTimeProfiler timeProfiler(name);
#else
#define PROFILE_TIME(name)
#endif // #ifdef DISABLE_PROFILER