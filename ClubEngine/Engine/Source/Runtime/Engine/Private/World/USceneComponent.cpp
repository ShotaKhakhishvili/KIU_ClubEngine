#include <Engine/World/USceneComponent.h>
#include <algorithm>
#include <ranges>

namespace CE
{

USceneComponent::USceneComponent(AActor* owner, const FTransform& newTransform)
    : Super(owner), worldTransform(newTransform), relativeTransform(newTransform)
{

}

USceneComponent* USceneComponent::GetParentComponent() const
{
    return parentComponent;
}

void USceneComponent::SetParentComponent(USceneComponent* parent)
{
    if (parent == parentComponent || this->IsAncestorOf(parent))
        return;

    const FTransform keptWorld = worldTransform;

    if (parentComponent != nullptr)
        std::erase(parentComponent->childComponents, this);

    parentComponent = parent;

    if (parent != nullptr)
        parent->childComponents.push_back(this);

    SetWorldTransform(keptWorld);
}

void USceneComponent::RemoveChildComponent(USceneComponent* child)
{
    if (child != nullptr && child->parentComponent == this)
        child->SetParentComponent(nullptr);
}

void USceneComponent::AddChildComponent(USceneComponent* child)
{
    if (child != nullptr)
        child->SetParentComponent(this);
}

bool USceneComponent::IsAncestorOf(const USceneComponent* other) const
{
    for (auto it = other; it != nullptr; it = it->parentComponent)
    {
        if (it == this)
            return true;
    }

    return false;
}

TArray<USceneComponent*> USceneComponent::GetChildren() const
{
    return childComponents;
}

void USceneComponent::OnDestroyed()
{
    Super::OnDestroyed();

    SetParentComponent(nullptr);

    for (USceneComponent* child : childComponents)
    {
        child->parentComponent = nullptr;
        child->relativeTransform = child->worldTransform;
    }

    childComponents.clear();
}


// --------------------------- Transforms Start ---------------------------

const FTransform& USceneComponent::GetWorldTransform()      const { return worldTransform;                  }
FVector USceneComponent::GetWorldLocation()                 const { return worldTransform.Translation;      }
FRotator USceneComponent::GetWorldRotation()                const { return worldTransform.GetRotator();     }
FVector USceneComponent::GetWorldScale()                    const { return worldTransform.Scale;            }

const FTransform& USceneComponent::GetRelativeTransform()   const { return relativeTransform;               }
FVector USceneComponent::GetRelativeLocation()              const { return relativeTransform.Translation;   }
FRotator USceneComponent::GetRelativeRotation()             const { return relativeTransform.GetRotator();  }
FVector USceneComponent::GetRelativeScale()                 const { return relativeTransform.Scale;         }

void USceneComponent::OnTransformUpdated() {}

void USceneComponent::SetWorldTransform(const FTransform& newTransform)
{
    if (parentComponent == nullptr)
    {
        SetRelativeTransform(newTransform);
        return;
    }

    const FTransform& parentWorld = parentComponent->worldTransform;

    FTransform relative;
    relative.Translation = parentWorld.InverseTransformPoint(newTransform.Translation);
    relative.Rotation    = Conjugate(parentWorld.Rotation) * newTransform.Rotation;
    relative.Scale       = DivideSafe(newTransform.Scale, parentWorld.Scale);

    SetRelativeTransform(relative);
}

void USceneComponent::SetWorldLocation(const FVector& newLocation)
{
    FTransform newTransform = worldTransform;
    newTransform.Translation = newLocation;
    SetWorldTransform(newTransform);
}

void USceneComponent::SetWorldRotation(const FRotator& newRotation)
{
    FTransform newTransform = worldTransform;
    newTransform.SetRotation(newRotation);
    SetWorldTransform(newTransform);
}

void USceneComponent::SetWorldScale(const FVector& newScale)
{
    FTransform newTransform = worldTransform;
    newTransform.Scale = newScale;
    SetWorldTransform(newTransform);
}

void USceneComponent::SetRelativeTransform(const FTransform& newTransform)
{
    relativeTransform = newTransform;
    UpdateWorldTransform();
}

void USceneComponent::SetRelativeLocation(const FVector& newLocation)
{
    relativeTransform.Translation = newLocation;
    UpdateWorldTransform();
}

void USceneComponent::SetRelativeRotation(const FRotator& newRotation)
{
    relativeTransform.SetRotation(newRotation);
    UpdateWorldTransform();
}

void USceneComponent::SetRelativeScale(const FVector& newScale)
{
    relativeTransform.Scale = newScale;
    UpdateWorldTransform();
}

void USceneComponent::UpdateWorldTransform()
{
    worldTransform = parentComponent != nullptr
        ? Combine(parentComponent->worldTransform, relativeTransform)
        : relativeTransform;

    OnTransformUpdated();

    for (USceneComponent* child : childComponents)
        child->UpdateWorldTransform();
}

// --------------------------- Transforms End ---------------------------

}