// Library includes:
#include <SDL.h>
// Local includes:
#include "game/game.h"
#include "logmanager/logmanager.h"
int main(int argc, char* argv[]) {
    Game& gameInstance = Game::GetInstance();
    if (!gameInstance.Initialise())
    {
        LogManager::GetInstance().Log(ERROR, "Game initialise failed!");
        return 1;
    }
    while (gameInstance.DoGameLoop())
    {
        // No body.
    }
    Game::DestroyInstance();
    LogManager::DestroyInstance();
    return 0;
}