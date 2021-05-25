#include <cstdio>

#include "bot-core-test/Tests_BitSetGet.h"
#include "bot-core-test/Tests_GameState.h"

int main()
{
	ut3::RunTests_BitGetSet();
	ut3::RunTests_GameState();

	return 0;
}