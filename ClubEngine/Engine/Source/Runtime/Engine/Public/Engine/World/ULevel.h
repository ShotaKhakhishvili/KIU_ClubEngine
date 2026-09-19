#pragma once 

#include <CoreUObject/UObject.h>
#include <CoreUObject/UObjectRegistry.h>

#include <Engine/World/UActorComponent.h>

#include <type_traits>

namespace CE
{

class UWorld;

class ULevel : public UObject
{
    GENERATED_BODY(ULevel, UObject)

public:

    UWorld* GetWorld() const;

    template<typename T, typename... Args> 
        requires std::is_base_of_v<UActorComponent, T>
    TObjectHandle<T> CreateComponent(Args&&... args)
    {
        return componentRegistry->Create<T>(std::forward<Args>(args)...);
    }

    template<typename T> 
        requires std::is_base_of_v<UActorComponent, T>
    T* ResolveComponent(TObjectHandle<T> componentHandle)
    {
        return componentRegistry->Resolve<T>(componentHandle);
    }

private:
    friend class UWorld;

    UWorld* world = nullptr;
    UObjectRegistry* componentRegistry;
    UObjectRegistry* actorRegistry;
};

}