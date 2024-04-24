#include "BotCore_PCH.h"
#include "bot-core/bot/Bot.h"

#include "bot-core/bot/BotBehaviorInitParams.h"
#include "bot-core/bot/IBotBehavior.h"
#include "bot-core/ut3-game/Game.h"
#include "bot-core/ut3-game/GameStateView.h"

namespace ut3 {
namespace bot {

using namespace std;

CBot::CBot(unique_ptr<IBotBehavior> botBehavior, char const* botName)
    : m_botBehavior(move(botBehavior))
    , m_timeout(90)
    , m_botName(botName)
    , m_isDebugEnabled(false)
{
}

CBot::~CBot()
{}

SOutputData CBot::FirstUpdate(SInputData initData)
{
    if (initData.m_oppTurnX >= 0)
    {
        game::MakeTurn(m_gameState, initData.m_oppTurnX, initData.m_oppTurnY);
    }

    InitializeBB();

    auto const turn = FindAndMakeTurn();
    return { turn[0], turn[1] };
}

SOutputData CBot::Update(SInputData turnData)
{
    game::MakeTurn(m_gameState, turnData.m_oppTurnX, turnData.m_oppTurnY);
    if (m_isDebugEnabled)
    {
        game::SGameStateView(m_gameState).Print();
    }

    auto const turn = FindAndMakeTurn();
    return { turn[0], turn[1] };
}

SVec2 CBot::FindAndMakeTurn()
{
    auto const turn = m_botBehavior->FindTurn(m_gameState);
    game::MakeTurn(m_gameState, turn[0], turn[1]);
    return turn;
}

void CBot::Reset()
{
    m_botBehavior->Shutdown();
    m_gameState = game::SGameState();
}

void CBot::InitializeBB()
{
    SBotBehaviorInitParams params;
    params.m_isDebugEnabled = m_isDebugEnabled;
    params.m_timeout = m_timeout;
    params.m_randomSeed = (size_t)time(time_t(NULL)); // TODO: pass seed

    m_botBehavior->Initialize(m_gameState, params);
}

} // bot
} // ut3