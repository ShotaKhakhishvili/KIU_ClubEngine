#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>
#include <CoreUObject/TObjectHandle.h>

#include <Engine/World/USceneComponent.h>

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

private:
    TObjectHandle<USceneComponent> root;

    UWorld* ownerWorld;

    void SetOwnerWorld(UWorld* newOwnerWorld);

    friend class UWorld;
};

}
