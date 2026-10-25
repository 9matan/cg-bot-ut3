#include <cstdio>

#include "bot-core-test/Tests_BitSetGet.h"
#include "bot-core-test/Tests_Game.h"
#include "bot-core-test/Tests_GameState.h"
#include "bot-core-test/Tests_GameStateHelper.h"

#include "bot-core-test/Tests_MinimaxBot.h"

#include "bot-core-test/playground/BotsPlaygroundCases.h"



#include "mimax/common/Vec2.h"
#include "bot-core/ut3-game/GameState.h"
#include "bot-core/bot/BbMcts.h"

void RunGameTests()
{
	ut3::game::RunTests_BitGetSet();
	ut3::game::RunTests_GameState();
	ut3::game::RunTests_GameStateHelper();
	ut3::game::RunTests_Game();
}

void RunBotTests()
{
	ut3::bot::RunTests_MinimaxBot();
}

SVec2 FindBestTurn(ut3::game::SGameState const& gameState)
{
	using namespace std::chrono_literals;

	auto bbMcts = std::make_unique<ut3::bot::CBbMcts>(0.3f, 30);
	ut3::bot::SBotBehaviorInitParams params;
	params.m_timeout = 15000ms;
	params.m_isDebugEnabled = true;
	bbMcts->Initialize(gameState, params);

	return bbMcts->FindTurn(gameState);
}

int main()
{
	RunGameTests();
	RunBotTests();

	//ut3::bot::Run_Playground_Cases();
	ut3::game::SGameState state{};
	ut3::game::MakeTurn(state, 4, 4);
	ut3::game::MakeTurn(state, 3, 3);
	ut3::game::MakeTurn(state, 2, 2);
	ut3::game::MakeTurn(state, 8, 8);
	ut3::game::MakeTurn(state, 6, 6);
	const auto turn = FindBestTurn(state);
	printf("%i %i", turn[0], turn[1]);

	return 0;
}