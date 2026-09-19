#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/TObjectHandle.h>

#include <Engine/World/USceneComponent.h>
#include <Engine/World/FObjectInitializer.h>
#include <Engine/World/ULevel.h>

#include <type_traits>

namespace CE
{

class UWorld;

class AActor : public UObject
{
    GENERATED_BODY(AActor, UObject)

public:
    explicit AActor(const FTransform& transform);
    ~AActor() override = default;

    virtual void Tick(float dt);

    TObjectHandle<USceneComponent> GetRootComponent() const;

    UWorld* GetWorld() const;
    ULevel* GetLevel() const;

protected:

    virtual void OnSpawned();

    template<typename T, typename... Args>
        requires std::is_base_of_v<UActorComponent,T>
    TObjectHandle<T> CreateDefaultSubobject(Args&&... args)
    {
        TObjectHandle<T> newSubobject = level->CreateComponent<T>(std::forward<Args>(args)...);
        ownedComponents.push_back(newSubobject);

        return newSubobject;
    }

    TObjectHandle<USceneComponent> rootComponent;

private:
    friend class UWorld;

    std::vector<TObjectHandle<UActorComponent>> ownedComponents;
    ULevel* level = FObjectInitializer::GetCurrent();
};

}
