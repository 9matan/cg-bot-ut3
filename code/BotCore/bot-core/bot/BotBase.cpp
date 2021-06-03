#include "BotCore_PCH.h"


#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameStateView.h"

#include "bot-core/bot/BotBase.h"

namespace ut3 {
namespace bot {

    SOutputData CBotBase::FirstUpdate(SInputData initData)
    {
        if (initData.m_oppTurnX >= 0)
        {
            return Update(initData);
        }
        else
        {
            auto const turn = FindTurn(m_gameState);
            game::MakeTurn(m_gameState, turn[0], turn[1]);
            return { turn[0], turn[1] };
        }
    }

    SOutputData CBotBase::Update(SInputData turnData)
    {
        game::MakeTurn(m_gameState, turnData.m_oppTurnX, turnData.m_oppTurnY);
        if (m_isDebugEnabled)
        {
            game::SGameStateView(m_gameState).Print();
        }

        auto const turn = FindTurn(m_gameState);

        game::MakeTurn(m_gameState, turn[0], turn[1]);
        return { turn[0], turn[1] };
    }

}
}