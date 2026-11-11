#include <Engine/World/UPrimitiveComponent.h>

#include "Engine/World/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{

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
void UPrimitiveComponent::SetWorldTransform(const FTransform& newTransform)
{
    Super::SetWorldTransform(newTransform);

    GetOwner()->GetWorld()->GetScene().UpdateTransform(primitiveSceneProxyHandle, newTransform);
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