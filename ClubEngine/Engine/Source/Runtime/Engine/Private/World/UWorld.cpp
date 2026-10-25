#include <Engine/World/UWorld.h>

#include <algorithm>

namespace CE
{

void UWorld::TickWorld(const float dt)
{
    persistentLevel->TickLevel(dt);

    for (ULevel* level : streamedLevels)
        level->TickLevel(dt);
}

ULevel* UWorld::GetLevelFromRegistryID(uint32 registryId) const
{
    if (persistentLevel->actorRegistry.GetRegistryId() == registryId ||
                persistentLevel->componentRegistry.GetRegistryId() == registryId)
        return persistentLevel;

    auto level = std::ranges::find_if(streamedLevels, [&registryId](ULevel* level)
    {
        return level->actorRegistry.GetRegistryId() == registryId ||
                level->componentRegistry.GetRegistryId() == registryId;
    });

    return level == streamedLevels.end() ?  nullptr : *level;
}

UWorld::~UWorld()
{
    for (const ULevel* level : streamedLevels)
        delete level;

    delete persistentLevel;
}
void UWorld::Destroy(AActor* actor)
{
    if(actor == nullptr || actor->isPendingDestroy)
        return;

    actor->isPendingDestroy = true;
    actor->OnDestroyed();

    ULevel* level = actor->level;

    for (auto component : actor->ownedComponents)
        component->OnDestroyed();

    for (auto component : actor->ownedComponents)
        level->componentRegistry.Destroy(component->GetHandle());


    actor->ownedComponents.clear();
    actor->rootComponent = nullptr;

    level->actorRegistry.Destroy(actor->GetHandle());
}
FScene& UWorld::GetScene()
{
    return scene;
}    

}