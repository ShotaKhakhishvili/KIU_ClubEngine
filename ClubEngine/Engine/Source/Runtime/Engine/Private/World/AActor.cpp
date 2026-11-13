#include <Engine/World/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

IMPLEMENT_CLASS(AActor)

AActor::AActor(const FTransform& transform)
{
}

USceneComponent* AActor::GetRootComponent() const
{
    return rootComponent;
}

void AActor::Tick(const float dt)
{
    (void)dt;
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