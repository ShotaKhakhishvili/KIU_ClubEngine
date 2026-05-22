#include <Engine/World/USceneComponent.h>

namespace CE
{

USceneComponent::USceneComponent(const FTransform& transform)
    : transform(transform)
{

}

void USceneComponent::SetComponentTransform(const FTransform& transform)
{
    this->transform = transform;
}

}