#include "BotCore_PCH.h"
#include "bot-core/bot/BbCustom.h"

namespace ut3 {
namespace bot {

CBbCustom::CBbCustom(FindTurnFunc findTurnFunc)
    : m_findTurnFunc(findTurnFunc)
{}

SVec2 CBbCustom::FindTurn(game::SGameState const& gameState)
{
    return m_findTurnFunc(gameState);
}

} // bot
} // ut3