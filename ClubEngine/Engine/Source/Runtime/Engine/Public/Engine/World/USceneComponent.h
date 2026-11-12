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

    void OnDestroyed() override;

    void SetWorldTransform(const FTransform& newTransform);
    const FTransform& GetWorldTransform() const;

    void SetWorldLocation(const FVector& newLocation);
    FVector GetWorldLocation() const;
    void SetWorldRotation(const FRotator& newRotation);
    FRotator GetWorldRotation() const;
    void SetWorldScale(const FVector& newScale);
    FVector GetWorldScale() const;

    void SetRelativeTransform(const FTransform& newTransform);
    const FTransform& GetRelativeTransform() const;

    void SetRelativeLocation(const FVector& newLocation);
    FVector GetRelativeLocation() const;
    void SetRelativeRotation(const FRotator& newRotation);
    FRotator GetRelativeRotation() const;
    void SetRelativeScale(const FVector& newScale);
    FVector GetRelativeScale() const;

    USceneComponent* GetParentComponent() const;
    void SetParentComponent(USceneComponent* parent);

    void RemoveChildComponent(USceneComponent* child);
    void AddChildComponent(USceneComponent* child);

    bool IsAncestorOf(const USceneComponent* other) const;
    TArray<USceneComponent*> GetChildren() const;

protected:
    virtual void OnTransformUpdated();

private:
    void UpdateWorldTransform();

    FTransform worldTransform;
    FTransform relativeTransform;

    USceneComponent* parentComponent = nullptr;
    TArray<USceneComponent*> childComponents;
};

}