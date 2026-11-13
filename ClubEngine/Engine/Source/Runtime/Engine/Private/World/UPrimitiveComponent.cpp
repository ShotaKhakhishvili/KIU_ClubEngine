#include <Engine/World/UPrimitiveComponent.h>

#include "Engine/World/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{

IMPLEMENT_CLASS(UPrimitiveComponent)

UPrimitiveComponent::UPrimitiveComponent(AActor* owner, const FTransform& transform)
    : Super(owner, transform)
{
    FPrimitiveSceneProxy primitiveSceneProxy;
    primitiveSceneProxy.localToWorld = static_cast<FMatrix4x4F>(transform.ToMatrix());

    primitiveSceneProxyHandle = GetOwner()->GetWorld()->GetScene().AddProxy(primitiveSceneProxy);
}

bool UPrimitiveComponent::IsVisible()const
{
    return isVisible;
}

void UPrimitiveComponent::SetVisible(bool newVisibility)
{
    isVisible = newVisibility;
    GetOwner()->GetWorld()->GetScene().SetVisible(primitiveSceneProxyHandle, isVisible);
}
void UPrimitiveComponent::OnTransformUpdated()
{
    GetOwner()->GetWorld()->GetScene().UpdateTransform(primitiveSceneProxyHandle, GetWorldTransform());
}
void UPrimitiveComponent::OnDestroyed()
{
    Super::OnDestroyed();

    GetOwner()->GetWorld()->GetScene().RemoveProxy(primitiveSceneProxyHandle);
}

FPrimitiveSceneProxyHandle UPrimitiveComponent::GetPrimitiveSceneProxy() const
{
    return primitiveSceneProxyHandle;
}

}