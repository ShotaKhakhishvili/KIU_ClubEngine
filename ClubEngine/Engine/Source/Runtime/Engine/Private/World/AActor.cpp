#include <Engine/World/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

AActor::AActor(const FTransform& transform)
    : AActor()
{
    rootComponent = level->CreateComponent<USceneComponent>(transform);
}

TObjectHandle<USceneComponent> AActor::GetRootComponent() const 
{
    return rootComponent;
}

void AActor::Tick(float dt)
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

void AActor::OnSpawned()
{

}

}