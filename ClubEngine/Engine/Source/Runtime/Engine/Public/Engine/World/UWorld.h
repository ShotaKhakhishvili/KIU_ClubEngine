#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObjectRegistry.h>

#include <Engine/World/AActor.h>
#include <Engine/World/UActorComponent.h>
#include <Engine/World/FObjectInitializer.h>

#include <vector>
#include <type_traits>

namespace CE
{

class WorldSystem;

class UWorld : public UObject
{
    GENERATED_BODY(UWorld, UObject)

public:

    template<typename T, typename... Args> requires std::is_base_of_v<AActor, T>
    TObjectHandle<T> SpawnActor(const FTransform& transform, Args&&... args)
    {
        FObjectInitializer initializer(&persistentLevel);

        TObjectHandle<AActor> handle = persistentLevel.actorRegistry.Create<T>(std::forward<Args>(args)...);
        AActor* actor = persistentLevel.actorRegistry.Resolve<AActor>(handle);

        if(!actor->rootComponent.IsValid())
        {
            actor->rootComponent = actor->CreateDefaultSubobject<USceneComponent>();
        }

        for(auto& component : actor->ownedComponents)
        {
            persistentLevel.ResolveComponent<UActorComponent>(component)->SetOwner(handle);
        }

        persistentLevel.ResolveComponent(actor->rootComponent)->SetWorldTransform(transform);
        actor->OnSpawned();

        return handle;
    }
    void Destroy(TObjectHandle<AActor>);

private:
    friend class WorldSystem;

    void TickWorld(float dt);

    ULevel persistentLevel;
};

}

