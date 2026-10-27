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
    explicit USceneComponent(AActor* owner, const FTransform& newTransform);

    void SetWorldTransform(const FTransform& newTransform);

    USceneComponent* GetParentComponent() const;
    void SetParentComponent(USceneComponent* parent);

    void RemoveChildComponent(USceneComponent* child);
    void AddChildComponent(USceneComponent* child);

    bool IsAncestorOf(const USceneComponent* other) const;
    TArray<USceneComponent*> GetChildren() const;

private:
    FTransform transform;

    USceneComponent* parentComponent = nullptr;
    TArray<USceneComponent*> childComponents;
};

}