#include <Engine/World/UActorComponent.h>

namespace CE
{

void UActorComponent::Tick(float dt)
{
    (void)dt;
}

void UActorComponent::SetOwner(TObjectHandle<AActor> owner)
{
    this->owner = owner;
}

}
