#include <Engine/World/USceneComponent.h>
#include <algorithm>

namespace CE
{

USceneComponent::USceneComponent(AActor* owner, const FTransform& newTransform)
    : Super(owner), transform(newTransform)
{

}

void USceneComponent::SetWorldTransform(const FTransform& newTransform)
{
    this->transform = newTransform;
}
USceneComponent* USceneComponent::GetParentComponent() const
{
    return parentComponent;
}

void USceneComponent::SetParentComponent(USceneComponent* parent)
{
    if (parent == parentComponent || this->IsAncestorOf(parent))
        return;

    if (parentComponent != nullptr)
    {
        parentComponent->RemoveChildComponent(this);
    }

    parentComponent = parent;
    if (parent != nullptr)
        parent->childComponents.push_back(this);
}
void USceneComponent::RemoveChildComponent(USceneComponent* child)
{
    if (child == nullptr || child == this)
        return;

    auto it = std::ranges::find(childComponents, child);

    if (it != childComponents.end())
    {
        (*it)->parentComponent = nullptr;
        childComponents.erase(it);
    }
}
void USceneComponent::AddChildComponent(USceneComponent* child)
{
    if (child == nullptr || child->parentComponent == this || child->IsAncestorOf(this))
        return;

    if (child->parentComponent)
        child->parentComponent->RemoveChildComponent(child);

    child->parentComponent = this;
    childComponents.push_back(child);
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

}