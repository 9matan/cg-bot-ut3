#pragma once

#include <chrono>

namespace ut3 {
namespace bot {

struct SBotBehaviorInitParams
{
    std::chrono::milliseconds m_timeout = std::chrono::milliseconds(90);
    size_t m_randomSeed = 0;
    bool m_isDebugEnabled = false;
};

} // bot
} // ut3