#include <Engine/World/UPrimitiveComponent.h>

namespace CE
{

UPrimitiveComponent::UPrimitiveComponent(FPrimitiveSceneProxyHandle inPrimitiveSceneProxy)
    : primitiveSceneProxy(inPrimitiveSceneProxy)
{}


bool UPrimitiveComponent::IsVisible()const
{
    return isVisible;
}
void UPrimitiveComponent::SetVisible(bool newVisibility)
{
    isVisible = newVisibility;
}

}