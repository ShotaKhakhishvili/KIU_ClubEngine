#include <Engine/World/USceneComponent.h>

namespace CE
{

USceneComponent::USceneComponent(const FTransform& transform)
    : transform(transform)
{
    
}

void USceneComponent::SetWorldTransform(const FTransform& transform)
{
    this->transform = transform;
}

}