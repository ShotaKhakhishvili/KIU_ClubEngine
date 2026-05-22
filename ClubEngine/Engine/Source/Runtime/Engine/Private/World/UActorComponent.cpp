#include <Engine/World/UActorComponent.h>

namespace CE
{

void UActorComponent::Update(float dt)
{
    (void)dt;
}

void UActorComponent::SetOwner(AActor* owner)
{
    this->owner = owner;
}

}
