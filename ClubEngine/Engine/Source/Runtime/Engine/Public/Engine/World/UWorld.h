#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObjectRegistry.h>

#include <Engine/World/AActor.h>

#include <vector>
#include <type_traits>

namespace CE
{

class WorldSystem;

class UWorld : public UObject
{
    GENERATED_BODY(UWorld, UObject)

public:

    template<typename T> requires std::is_base_of_v<AActor, T>
    TObjectHandle<T> SpawnActor(const FTransform& transform)
    {
        return actorRegistry.Create<T>(transform);
    }
    void Destroy(TObjectHandle<AActor>);

private:
    UObjectRegistry actorRegistry;

    friend class WorldSystem;

    void TickWorld(float dt);
};

}

