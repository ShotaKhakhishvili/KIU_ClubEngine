#include <Engine/World/UWorld.h>

#include <algorithm>

namespace CE
{

IMPLEMENT_CLASS(UWorld)

AActor* UWorld::SpawnActor(UClass* actorClass, const FTransform& transform, const FActorSpawnParameters& params)
{
    if (actorClass == nullptr || !actorClass->IsChildOf(AActor::StaticClass()))
    {
        CE_LOG(Error, "Tried To Spawn Non-Actor Class {}.", actorClass ? actorClass->GetName() : "null");
        return nullptr;
    }

    AActor* actor = nullptr;

    {
        FObjectInitializer initializer(persistentLevel);

        const UObjectHandle handle = persistentLevel->actorRegistry.Create(actorClass);

        if (!handle.IsValid())
            return nullptr;

        actor = static_cast<AActor*>(persistentLevel->actorRegistry.Resolve(handle));
    }

    if (params.name != nullptr)
        actor->SetName(params.name);

    if (!actor->rootComponent)
        actor->rootComponent = actor->CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");

    actor->rootComponent->SetWorldTransform(transform);
    actor->RegisterAllComponents();

    if (!params.deferConstruction)
        actor->FinishSpawning(transform);

    return actor;
}

void UWorld::TickWorld(const float dt)
{
    persistentLevel->TickLevel(dt);

    for (ULevel* level : streamedLevels)
        level->TickLevel(dt);

    persistentLevel->componentRegistry.FlushPendingDestroy();
    persistentLevel->actorRegistry.FlushPendingDestroy();
    for (ULevel* level : streamedLevels)
    {
        level->componentRegistry.FlushPendingDestroy();
        level->actorRegistry.FlushPendingDestroy();
    }

    std::erase_if(viewTargets, [this](const FViewTarget& viewTarget)
    {
        return ResolveComponent(viewTarget.camera) == nullptr;
    });
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
    if(actor == nullptr)
        return;

    if (actor->IsPendingDestroy())
        return;

    ULevel* level = actor->level;
    level->actorRegistry.Destroy(actor->GetHandle());

    for (const auto& component : actor->ownedComponents)
        level->componentRegistry.Destroy(component->GetHandle());

    actor->OnDestroyed();
    actor->UnregisterAllComponents();

    for (const auto& component : actor->ownedComponents)
        component->OnDestroyed();

    actor->ownedComponents.clear();
    actor->rootComponent = nullptr;
}

FScene& UWorld::GetScene()
{
    return scene;
}    

void UWorld::AddViewTarget(UCameraComponent* camera, const float x, const float y, const float width, const float height)
{
    if (camera == nullptr || camera->GetOwner()->GetWorld() != this)
    {
        CE_LOG(Warning, "Tried To Add A Null Or Foreign Camera As A View Target.");
        return;
    }

    viewTargets.push_back({ TObjectHandle<UCameraComponent>(camera->GetHandle()), x, y, width, height });
}

void UWorld::RemoveViewTarget(const UCameraComponent* camera)
{
    if (camera == nullptr)
        return;

    const TObjectHandle<UCameraComponent> handle(camera->GetHandle());

    std::erase_if(viewTargets, [&handle](const FViewTarget& viewTarget)
    {
        return viewTarget.camera == handle;
    });
}

const TArray<FViewTarget>& UWorld::GetViewTargets() const
{
    return viewTargets;
}

}