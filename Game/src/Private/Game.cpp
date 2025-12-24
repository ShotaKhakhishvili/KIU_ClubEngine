#include "Game.h"

#include "Player.h"
#include "SABook.h"
#include "Barrier.h"
#include "GameLogic.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern Font* FONT_MAIN;

void Game::Init()
{
    auto* instObj = World::CreateActor<InstancedObject>("Cube.txt", "1.png", "default.frag");

    instObj->AddInstance(glm::vec3(0,0,0));
    instObj->SetScale(0,glm::vec3(100,3,100));
    instObj->SetRotation(0,glm::vec3(0,0,0));

    World::AddLight({-4,2,1}, {1,1,1}, 5,4);
    World::AddLight({0,2,1}, {1,1,1}, 10,4);
    World::AddLight({4, 2,1}, {1,1,1}, 5,5);

    auto* player = World::CreateActor<Player>();
    auto* gameLogic = World::CreateActor<GameLogic>(player);
}
