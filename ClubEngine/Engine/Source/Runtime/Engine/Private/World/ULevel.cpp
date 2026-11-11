#include <Engine/World/ULevel.h>
#include <Engine/World/UWorld.h>

namespace CE
{

ULevel::ULevel(UWorld* inWorld)
{
    world = inWorld;
}

UWorld* ULevel::GetWorld() const
{
    return world;
}

bool ULevel::IsAlive(UObjectHandle handle) const
{
    return actorRegistry.IsAlive(handle) || componentRegistry.IsAlive(handle);
}

void ULevel::TickLevel(const float dt)
{
    actorRegistry.ForEach([dt](UObject& object)
    {
        AActor& actor = static_cast<AActor&>(object);
        actor.Tick(dt);
    });
    componentRegistry.ForEach([dt](UObject& object)
    {
        UActorComponent& component = static_cast<UActorComponent&>(object);
        if (component.GetOwner())
            component.Tick(dt);
    });
}

}