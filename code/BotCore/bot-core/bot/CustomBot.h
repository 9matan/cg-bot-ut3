#pragma once

#include <functional>
#include <string>

#include "bot-core/bot/BotBase.h"

namespace ut3 {
namespace bot {

    class CCustomBot : public CBotBase
    {
    public:
        using FindTurnFunc = std::function<SVec2(game::SGameState const& gameState, int const myPlayer, bool const debugEnabled)>;

        CCustomBot(FindTurnFunc findTurnFunc, char const* botName);

    protected:
        SVec2 FindTurn(game::SGameState const& gameState) override;

    private:
        FindTurnFunc m_findTurnFunc;
    };

}
}