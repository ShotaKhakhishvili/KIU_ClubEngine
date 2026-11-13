#include <Engine/World/UActorComponent.h>

#include "Engine/World/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{

IMPLEMENT_CLASS(UActorComponent)

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
    return owner == nullptr || !owner->GetLevel()->IsAlive(GetHandle());
}

void UActorComponent::RegisterComponent()
{
    if (registered || owner == nullptr || owner->GetWorld() == nullptr)
        return;

    registered = true;
    OnRegister();
}

void UActorComponent::UnregisterComponent()
{
    if (!registered)
        return;

    OnUnregister();
    registered = false;
}

bool UActorComponent::IsRegistered() const
{
    return registered;
}

void UActorComponent::OnRegister()
{
}

void UActorComponent::OnUnregister()
{
}

}
