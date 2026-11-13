#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/TObjectHandle.h>

#include <Engine/World/USceneComponent.h>
#include <Engine/World/FObjectInitializer.h>
#include <Engine/World/ULevel.h>
#include <Engine/EngineTypes.h>

#include <type_traits>

namespace CE
{

class UWorld;

class AActor : public UObject
{
    GENERATED_BODY(AActor, UObject)

public:
    AActor() = default;
    ~AActor() override = default;

    void FinishSpawning(const FTransform& transform);

    virtual void Tick(float dt);

    void RegisterAllComponents();
    void UnregisterAllComponents();

    USceneComponent* GetRootComponent() const;

    UWorld* GetWorld() const;
    ULevel* GetLevel() const;

    bool IsPendingDestroy() const;

protected:
    virtual void OnSpawned();
    virtual void OnDestroyed();

    template<typename T>
        requires std::is_base_of_v<UActorComponent,T>
    T* CreateDefaultSubobject(const char* name);

    USceneComponent* rootComponent = nullptr;

private:
    friend class UWorld;

    TArray<UActorComponent*> ownedComponents;
    ULevel* level = FObjectInitializer::GetCurrent();

    EActorState state = EActorState::Constructed;
};

}

#include "AActor.inl"