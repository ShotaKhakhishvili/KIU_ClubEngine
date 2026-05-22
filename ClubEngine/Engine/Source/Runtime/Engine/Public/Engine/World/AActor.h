#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObject.h>

#include <Engine/World/USceneComponent.h>

namespace CE
{

class AActor : public UObject
{
    GENERATED_BODY(AActor, UObject)

public:
    explicit AActor(const FTransform& transform);
    ~AActor() override = default;

    USceneComponent* GetRootComponent() const;

private:
    USceneComponent* root;
};

}
