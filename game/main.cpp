#include "config/config.h"
#include "engine/game.h"
#include "engine/logmanager/logmanager.h"

using namespace Engine;

int main(int argc, char* argv[]) {

    Game& gameInstance = Game::GetInstance();
    if (!gameInstance.Initialise())
    {
        LogManager::GetInstance().Log(ERROR, "Game initialise failed!");
        return 1;
    }

    while (gameInstance.DoGameLoop()) {}

    Game::DestroyInstance();
    LogManager::DestroyInstance();
    return 0;
}