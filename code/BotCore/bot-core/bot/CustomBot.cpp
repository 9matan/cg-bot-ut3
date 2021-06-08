#include "BotCore_PCH.h"
#include "bot-core/bot/CustomBot.h"

namespace ut3 {
namespace bot {


    CCustomBot::CCustomBot(FindTurnFunc findTurnFunc, std::string const& botName)
        : m_findTurnFunc(findTurnFunc)
        , m_botName(botName)
    {}

    SVec2 CCustomBot::FindTurn(game::SGameState const& gameState)
    {
        return m_findTurnFunc(gameState, m_myPlayer, m_isDebugEnabled);
    }

}
}