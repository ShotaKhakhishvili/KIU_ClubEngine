#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObjectRegistry.h>
#include <CoreUObject/TSubclassOf.h>

#include <Renderer/Scene/FScene.h>

#include <Engine/World/AActor.h>
#include <Engine/World/UActorComponent.h>
#include <Engine/World/FActorSpawnParameters.h>

#include <vector>
#include <type_traits>

namespace CE
{

class WorldSystem;

class UWorld : public UObject
{
    GENERATED_BODY(UWorld, UObject)

public:
    ~UWorld();

    AActor* SpawnActor(UClass* actorClass, const FTransform& transform, const FActorSpawnParameters& params = {});

    template<typename T>
        requires std::is_base_of_v<AActor, T>
    T* SpawnActor(const FTransform& transform, const FActorSpawnParameters& params = {});

    template<typename T>
        requires std::is_base_of_v<AActor, T>
    T* SpawnActor(TSubclassOf<T> actorClass, const FTransform& transform, const FActorSpawnParameters& params = {});

    template<typename T>
        requires std::is_base_of_v<AActor, T>
    T* SpawnActorDeferred(const FTransform& transform);

    template<typename T>
        requires std::is_base_of_v<AActor, T>
    T* SpawnActorDeferred(TSubclassOf<T> actorClass, const FTransform& transform);

    void Destroy(AActor* actor);

    template<typename T>
        requires std::is_base_of_v<UActorComponent, T>
    void DestroyComponent(T* component);

    template<typename T>
        requires std::is_base_of_v<AActor, T>
    T* ResolveActor(TObjectHandle<T> handle) const;

    template<typename T>
    requires std::is_base_of_v<UActorComponent, T>
    T* ResolveComponent(TObjectHandle<T> handle) const;

    FScene& GetScene();

private:
    friend class WorldSystem;

    ULevel* GetLevelFromRegistryID(uint32 registryId) const;

    void TickWorld(float dt);

    ULevel* persistentLevel = new ULevel(this);
    TArray<ULevel*> streamedLevels;

    FScene scene;
};

}

#include "UWorld.inl"

