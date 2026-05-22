#pragma once 

#include <Core/ClubCore.h>
#include <Core/Math/Transform.h>

#include <Engine/World/UActorComponent.h>

namespace CE
{

class USceneComponent : public UActorComponent
{
    GENERATED_BODY(USceneComponent, UActorComponent)

public:
    explicit USceneComponent(const FTransform& transform);

    void SetComponentTransform(const FTransform& transform);

private:
    FTransform transform;
};

}