#include <Engine/World/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

IMPLEMENT_CLASS(AActor)

USceneComponent* AActor::GetRootComponent() const
{
    return rootComponent;
}

void AActor::FinishSpawning(const FTransform& transform)
{
    if (state != EActorState::Constructed || IsPendingDestroy())
        return;

    state = EActorState::Spawned;
    rootComponent->SetWorldTransform(transform);

    OnSpawned();
}
void AActor::Tick(const float dt)
{
    (void)dt;
}

void AActor::RegisterAllComponents()
{
    for (UActorComponent* component : ownedComponents)
        component->RegisterComponent();
}

void AActor::UnregisterAllComponents()
{
    for (UActorComponent* component : ownedComponents)
        component->UnregisterComponent();
}

UWorld* AActor::GetWorld() const
{
    return level ? level->GetWorld() : nullptr;
}

ULevel* AActor::GetLevel() const
{
    return level;
}
bool AActor::IsPendingDestroy() const
{
    return level && !level->IsAlive(GetHandle());
}

void AActor::OnSpawned()
{

}

void AActor::OnDestroyed()
{
}

}