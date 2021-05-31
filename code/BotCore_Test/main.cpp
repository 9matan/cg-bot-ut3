#include <cstdio>

#include "bot-core-test/Tests_BitSetGet.h"
#include "bot-core-test/Tests_Game.h"
#include "bot-core-test/Tests_GameState.h"
#include "bot-core-test/Tests_GameStateHelper.h"

int main()
{
	ut3::game::RunTests_BitGetSet();
	ut3::game::RunTests_GameState();
	ut3::game::RunTests_Game();

	ut3::game::RunTests_GameStateHelper();

	return 0;
}