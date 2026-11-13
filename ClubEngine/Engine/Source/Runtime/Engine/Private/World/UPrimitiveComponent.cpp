#include <Engine/World/UPrimitiveComponent.h>

#include "Engine/World/AActor.h"
#include "Engine/World/UWorld.h"

namespace CE
{

IMPLEMENT_CLASS(UPrimitiveComponent)

bool UPrimitiveComponent::IsVisible()const
{
    return isVisible;
}

void UPrimitiveComponent::SetVisible(bool newVisibility)
{
    isVisible = newVisibility;

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().SetVisible(primitiveSceneProxyHandle, isVisible);
}

void UPrimitiveComponent::OnTransformUpdated()
{
    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdateTransform(primitiveSceneProxyHandle, GetWorldTransform());
}

FPrimitiveSceneProxyHandle UPrimitiveComponent::GetPrimitiveSceneProxy() const
{
    return primitiveSceneProxyHandle;
}

void UPrimitiveComponent::OnRegister()
{
    Super::OnRegister();

    FPrimitiveSceneProxy primitiveSceneProxy;
    primitiveSceneProxy.localToWorld = static_cast<FMatrix4x4F>(GetWorldTransform().ToMatrix());
    primitiveSceneProxy.isVisible = isVisible;

    primitiveSceneProxyHandle = GetOwner()->GetWorld()->GetScene().AddProxy(primitiveSceneProxy);
}

void UPrimitiveComponent::OnUnregister()
{
    GetOwner()->GetWorld()->GetScene().RemoveProxy(primitiveSceneProxyHandle);
    primitiveSceneProxyHandle = {};

    Super::OnUnregister();
}

}