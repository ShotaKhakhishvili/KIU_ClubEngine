#include "Game.h"

void Game::Init()
{
    glm::vec4 lightCol = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

    InstancedObject* instObj = World::CreateActor<InstancedObject>("Cube.txt", "1.png", "default.frag");

    instObj->AddInstance(glm::vec3(0,-2,0));
    instObj->SetScale(0,glm::vec3(100,100,3));
    instObj->SetRotation(0,glm::vec3(90,0,0));
    instObj->SetLightSource(lightPos, lightCol);
}