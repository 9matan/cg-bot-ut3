#include <cstdio>

#include "bot-core-test/Tests_BitSetGet.h"
#include "bot-core-test/Tests_Game.h"
#include "bot-core-test/Tests_GameState.h"
#include "bot-core-test/Tests_GameStateHelper.h"

#include "bot-core-test/Tests_MinimaxBot.h"

#include "bot-core-test/playground/BotsPlaygroundCases.h"

void RunTests()
{
	ut3::game::RunTests_BitGetSet();
	ut3::game::RunTests_GameState();
	ut3::game::RunTests_GameStateHelper();
	ut3::game::RunTests_Game();

	ut3::RunTests_MinimaxBot();
}

int main()
{
	RunTests();
	ut3::Run_Playground_Cases();

	return 0;
}