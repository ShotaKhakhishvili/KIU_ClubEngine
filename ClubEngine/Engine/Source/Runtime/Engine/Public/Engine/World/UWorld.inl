#pragma once
#include <Engine/World/AActor.h>
#include <Engine/World/FObjectInitializer.h>
#include <Engine/World/UActorComponent.h>
#include <Engine/World/USceneComponent.h>
#include <Core/Math/Transform.h>
#include <CoreUObject/TObjectHandle.h>
#include <Engine/World/FObjectInitializer.h>

#include <type_traits>
#include <utility>

namespace CE
{

template<typename T>
    requires std::is_base_of_v<AActor, T>
T* UWorld::SpawnActor(const FTransform& transform, const FActorSpawnParameters& params)
{
    static_assert(!std::is_abstract_v<T> && std::is_default_constructible_v<T>,
                  "Actors must be default constructible to be spawned");

    return static_cast<T*>(SpawnActor(T::StaticClass(), transform, params));
}

template<typename T>
    requires std::is_base_of_v<AActor, T>
T* UWorld::SpawnActor(TSubclassOf<T> actorClass, const FTransform& transform, const FActorSpawnParameters& params)
{
    return static_cast<T*>(SpawnActor(actorClass.Get(), transform, params));
}

template<typename T>
    requires std::is_base_of_v<AActor, T>
T* UWorld::SpawnActorDeferred(const FTransform& transform)
{
    return SpawnActor<T>(transform, FActorSpawnParameters{ .deferConstruction = true });
}

template<typename T>
    requires std::is_base_of_v<AActor, T>
T* UWorld::SpawnActorDeferred(TSubclassOf<T> actorClass, const FTransform& transform)
{
    return SpawnActor<T>(actorClass, transform, FActorSpawnParameters{ .deferConstruction = true });
}

template<typename T>
    requires std::is_base_of_v<UActorComponent, T>
void UWorld::DestroyComponent(T* component)
{
    if (!component)
        return;

    ULevel* level = GetLevelFromRegistryID(component->GetHandle().GetRegistryID());

    if(!level || !level->IsAlive(component->GetHandle()))
        return;

    if (component == component->owner->GetRootComponent())
    {
        Destroy(component->owner);
        return;
    }

    level->componentRegistry.Destroy(component->GetHandle());
    component->UnregisterComponent();

    component->OnDestroyed();
    std::erase(component->owner->ownedComponents, component);
}
template <typename T>
    requires std::is_base_of_v<AActor, T>
T* UWorld::ResolveActor(TObjectHandle<T> handle) const
{
    return GetLevelFromRegistryID(handle.GetRaw().GetRegistryID())->ResolveActor(handle);
}

template <typename T>
    requires std::is_base_of_v<UActorComponent, T>
T* UWorld::ResolveComponent(TObjectHandle<T> handle) const
{
    return GetLevelFromRegistryID(handle.GetRaw().GetRegistryID())->ResolveComponent(handle);
}

}