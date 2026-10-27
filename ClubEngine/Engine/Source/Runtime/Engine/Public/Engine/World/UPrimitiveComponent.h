#pragma once

#include <Asset/UStaticMesh.h>

#include <Renderer/Scene/FScene.h>

#include <Engine/World/USceneComponent.h>

namespace CE
{

class UPrimitiveComponent : public USceneComponent
{
    GENERATED_BODY(UPrimitiveComponent, USceneComponent)

public:
    explicit UPrimitiveComponent(AActor* owner, const FTransform& transform);

    bool IsVisible()const;
    void SetVisible(bool newVisibility);

protected:
    FPrimitiveSceneProxyHandle GetPrimitiveSceneProxy() const;

private:
    bool isVisible = true;

    FPrimitiveSceneProxyHandle primitiveSceneProxyHandle;
};

}