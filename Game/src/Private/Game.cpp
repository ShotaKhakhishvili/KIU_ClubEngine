#include "Game.h"

#include "Player.h"
#include "GameLogic.h"
#include "Path.h"
#include "Audio.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern Font* FONT_MAIN;
extern GLFWwindow* GAME_WINDOW;

void Game::Init()
{
    auto* floor = World::CreateActor<Object>("Cube.txt", "1.png", "default.frag");

    floor->SetScale(glm::vec3(50000,0.1,50000));
    floor->SetRotation(glm::vec3(0,0,0));
    floor->SetPosition({0,0,0});

    Audio::Init();

    auto* player = World::CreateActor<Player>(GAME_WINDOW);
    auto* gameLogic = World::CreateActor<GameLogic>(player);

    player->SetGameLogic(gameLogic);
}
