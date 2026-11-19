#include <Engine/Components/UPrimitiveComponent.h>

#include <Engine/Actors/AActor.h>
#include <Engine/World/UWorld.h>

namespace CE
{

IMPLEMENT_CLASS(UPrimitiveComponent)

void UPrimitiveComponent::OnTransformUpdated()
{
    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdatePrimitiveTransform(primitiveSceneProxyHandle, GetWorldTransform());
}
void UPrimitiveComponent::OnVisibilityChanged()
{
    Super::OnVisibilityChanged();

    if (IsRegistered())
        GetOwner()->GetWorld()->GetScene().UpdatePrimitiveVisibility(primitiveSceneProxyHandle, IsVisible());
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
    primitiveSceneProxy.isVisible = IsVisible();

    primitiveSceneProxyHandle = GetOwner()->GetWorld()->GetScene().AddPrimitive(primitiveSceneProxy);
}

void UPrimitiveComponent::OnUnregister()
{
    GetOwner()->GetWorld()->GetScene().RemovePrimitive(primitiveSceneProxyHandle);
    primitiveSceneProxyHandle = {};

    Super::OnUnregister();
}

}