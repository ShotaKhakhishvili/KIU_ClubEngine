#include <Engine/World/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

AActor::AActor(const FTransform& transform)
    : AActor()
{
    root = ownerWorld->CreateComponent<USceneComponent>(transform);
}

TObjectHandle<USceneComponent> AActor::GetRootComponent() const 
{
    return root;
}

void AActor::SetOwnerWorld(UWorld* newOwnerWorld)
{
    this->ownerWorld = newOwnerWorld;
}

void AActor::Tick(float dt)
{
    (void)dt;
}

}