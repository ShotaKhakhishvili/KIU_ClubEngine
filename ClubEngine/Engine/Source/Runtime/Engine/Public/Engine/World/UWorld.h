#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObjectRegistry.h>

#include <Engine/World/AActor.h>
#include <Engine/World/UActorComponent.h>

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
    TObjectHandle<T> SpawnActor(Args&&... args)
    {
        TObjectHandle<T> newActorHandle = actorRegistry.Create<T>(std::forward<Args>(args)...);

        AActor* newActor = actorRegistry.Resolve(newActorHandle);
        newActor->SetOwnerWorld(this);

        return newActorHandle;
    }
    void Destroy(TObjectHandle<AActor>);

    template<typename T, typename... Args> requires std::is_base_of_v<UActorComponent, T>
    TObjectHandle<T> CreateComponent(Args&&... args)
    {
        return componentRegistry.Create<T>(std::forward<Args>(args)...);
    }
    void DestroyComponent(TObjectHandle<UActorComponent>);

private:
    UObjectRegistry actorRegistry;
    UObjectRegistry componentRegistry;

    friend class WorldSystem;

    void TickWorld(float dt);
};

}

