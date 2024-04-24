#include "BotCore_PCH.h"
#include "bot-core/bot/BotFactory.h"

#include "bot-core/bot/BbCustom.h"
#include "bot-core/bot/BbMcts.h"
#include "bot-core/bot/BbMinimax.h"
#include "bot-core/bot/BbRandom.h"

namespace ut3 {
namespace bot {

CBot* CreateCustomBot(CBbCustom::FindTurnFunc func, char* const botName)
{
    auto bb = std::make_unique<CBbCustom>(func);
    return new CBot(std::move(bb), botName);
}

CBot* CreateMctsBot(float const explorationParam, size_t const maxDepth, char* const botName)
{
    auto bb = std::make_unique<CBbMcts>(explorationParam, maxDepth);
    return new CBot(std::move(bb), botName);
}

CBot* CreateMinimaxBot(char* const botName)
{
    auto bb = std::make_unique<CBbMinimax>();
    return new CBot(std::move(bb), botName);
}

CBot* CreateRandomBot(char* const botName)
{
    auto bb = std::make_unique<CBbRandom>();
    return new CBot(std::move(bb), botName);
}

} // bot
} // ut3