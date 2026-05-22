#include <Engine/World/AActor.h>

namespace CE
{

AActor::AActor(const FTransform& transform)
    : AActor()
{
    root = new USceneComponent(transform);
}

USceneComponent* AActor::GetRootComponent() const {return root;}

}