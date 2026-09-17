#include <Engine/Systems/WorldSystem.h>

namespace CE
{

const char* WorldSystem::GetName() const noexcept 
{
    return "WorldSystem"; 
}

int WorldSystem::Update(float dt)
{
    world.TickWorld(dt);

    return 0;
}

bool WorldSystem::Init()
{
    CE_LOG(Info, "World was initialized");

    return true;
}

}