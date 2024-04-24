#pragma once

#include "bot-core/InputData.h"
#include "bot-core/OutputData.h"

namespace ut3 {
namespace bot {

    class IBot
    {
    public:
        virtual ~IBot() = default;

        virtual SOutputData FirstUpdate(SInputData initData) = 0;
        virtual SOutputData Update(SInputData turnData) = 0;
        virtual void SetDebugIsEnabled(bool const enableDebug) = 0;
        virtual char const* GetBotName() const = 0;
        virtual void Reset() = 0;
    };

} // bot
} // ut3