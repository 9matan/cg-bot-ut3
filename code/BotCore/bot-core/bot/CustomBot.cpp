#include "BotCore_PCH.h"
#include "bot-core/bot/CustomBot.h"

namespace ut3 {
namespace bot {


    CCustomBot::CCustomBot(FindTurnFunc findTurnFunc, char const* botName)
        : CBotBase(botName)
        , m_findTurnFunc(findTurnFunc)
    {}

    SVec2 CCustomBot::FindTurn(game::SGameState const& gameState)
    {
        return m_findTurnFunc(gameState, m_myPlayer, m_isDebugEnabled);
    }

}
}