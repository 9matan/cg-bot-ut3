#include "BotUtility_PCH.h"

#include "bot-core/ut3-game/GameState.h"

#include "bot-utility/GameStateGenerator.h"

int main()
{
    using namespace ut3;

    ut3::utility::CGameStateDepthGenerator generator(game::SGameState{}, 2);

    std::cout << generator.Generate().size();

    return 0;
}