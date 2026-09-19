#include <Engine/World/UWorld.h>

namespace CE
{

void UWorld::TickWorld(float dt)
{
    persistentLevel.actorRegistry.ForEach([dt](UObject& object)
    {
        AActor& actor = static_cast<AActor&>(object);
        actor.Tick(dt);
        CE_LOG(Info, "Actor Was Ticked With Tick: {}", dt);
    });
}

void UWorld::Destroy(TObjectHandle<AActor> Handle)
{
    if(Handle.IsValid())
        persistentLevel.actorRegistry.Destroy(Handle.GetRaw());
}

}