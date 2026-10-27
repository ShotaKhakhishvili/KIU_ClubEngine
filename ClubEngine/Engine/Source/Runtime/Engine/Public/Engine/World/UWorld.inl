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

template<typename T, typename... Args>
    requires std::is_base_of_v<AActor, T>
T* UWorld::SpawnActor(const FTransform& transform, Args&&... args)
{
    FObjectInitializer initializer(persistentLevel);

    TObjectHandle<T> handle = persistentLevel->actorRegistry.Create<T>(transform, std::forward<Args>(args)...);
    AActor* actor = persistentLevel->actorRegistry.Resolve<AActor>(handle);

    if(!actor->rootComponent)
    {
        actor->rootComponent = actor->CreateDefaultSubobject<USceneComponent>(transform);
    }

    for(auto& component : actor->ownedComponents)
    {
        component->owner = actor;
    }

    actor->rootComponent->SetWorldTransform(transform);
    actor->OnSpawned();

    return persistentLevel->actorRegistry.Resolve<T>(handle);
}

template<typename T>
    requires std::is_base_of_v<UActorComponent, T>
void UWorld::DestroyComponent(T* component)
{
    if(!component)
        return;

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