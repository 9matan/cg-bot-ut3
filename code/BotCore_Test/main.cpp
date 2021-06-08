#include <cstdio>

#include "bot-core-test/Tests_BitSetGet.h"
#include "bot-core-test/Tests_Game.h"
#include "bot-core-test/Tests_GameState.h"
#include "bot-core-test/Tests_GameStateHelper.h"

#include "bot-core-test/Tests_MinimaxBot.h"

#include "bot-core-test/playground/BotsPlaygroundCases.h"

void RunGameTests()
{
	ut3::game::RunTests_BitGetSet();
	ut3::game::RunTests_GameState();
	ut3::game::RunTests_GameStateHelper();
	ut3::game::RunTests_Game();
}

void RundBotTests()
{
	ut3::bot::RunTests_MinimaxBot();
}

int main()
{
	RunGameTests();
	RundBotTests();

	ut3::bot::Run_Playground_Cases();

	return 0;
}