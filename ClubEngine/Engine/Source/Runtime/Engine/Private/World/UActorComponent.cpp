#include <Engine/World/UActorComponent.h>

#include "Engine/World/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{

IMPLEMENT_CLASS(UActorComponent)

UActorComponent::UActorComponent(AActor* owner)
    : owner(owner)
{
}
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
bool UActorComponent::IsPendingDestroy() const
{
    return !owner->GetLevel()->IsAlive(GetHandle());
}

}
