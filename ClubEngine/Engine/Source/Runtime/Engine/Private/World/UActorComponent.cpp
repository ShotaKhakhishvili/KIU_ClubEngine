#include <Engine/World/UActorComponent.h>

#include "Engine/World/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{

void UActorComponent::Tick(const float dt)
{
    (void)dt;
}
void UActorComponent::OnDestroyed()
{

}

AActor* UActorComponent::GetOwner() const
{
    return owner;
}

}
