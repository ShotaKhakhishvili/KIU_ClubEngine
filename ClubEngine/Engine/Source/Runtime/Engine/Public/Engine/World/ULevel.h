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

    explicit ULevel(UWorld* world);

    UWorld* GetWorld() const;

    template<typename T, typename... Args>
        requires std::is_base_of_v<UActorComponent, T>
    T* CreateComponent(Args&&... args);

    template<typename T> 
        requires std::is_base_of_v<UActorComponent, T>
    T* ResolveComponent(T* componentHandle);

    template<typename T>
        requires std::is_base_of_v<AActor, T>
    T* ResolveActor(TObjectHandle<T> componentHandle);

private:
    friend class UWorld;

    void TickLevel(float dt);

    UWorld* world = nullptr;
    UObjectRegistry actorRegistry;
    UObjectRegistry componentRegistry;
};

}

#include "ULevel.inl"