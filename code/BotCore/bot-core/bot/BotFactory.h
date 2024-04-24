#pragma once

#include "bot-core/bot/BbCustom.h"
#include "bot-core/bot/Bot.h"

namespace ut3 {
namespace bot {

CBot* CreateCustomBot(CBbCustom::FindTurnFunc func, char* const botName = "Custom");
CBot* CreateMctsBot(float const explorationParam, size_t const maxDepth, char* const botName = "Mcts");
CBot* CreateMinimaxBot(char* const botName = "Minimax");
CBot* CreateRandomBot(char* const botName = "Random");

} // bot
} // ut3