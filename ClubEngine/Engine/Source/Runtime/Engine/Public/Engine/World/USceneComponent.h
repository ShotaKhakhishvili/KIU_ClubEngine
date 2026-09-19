#pragma once 

#include <Core/ClubCore.h>
#include <Core/Math/Transform.h>

#include <CoreUObject/TObjectHandle.h>

#include <Engine/World/UActorComponent.h>

#include <vector>

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

    TObjectHandle<USceneComponent> parentComponent;
    std::vector<TObjectHandle<USceneComponent>> childComponents;
};

}