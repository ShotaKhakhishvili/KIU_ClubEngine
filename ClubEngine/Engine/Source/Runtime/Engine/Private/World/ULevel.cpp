#include <Engine/World/ULevel.h>
#include <Engine/World/UWorld.h>

namespace CE
{

ULevel::ULevel(UWorld* inWorld)
    : ULevel()
{
    world = inWorld;
}

UWorld* ULevel::GetWorld() const
{
    return world;
}

}