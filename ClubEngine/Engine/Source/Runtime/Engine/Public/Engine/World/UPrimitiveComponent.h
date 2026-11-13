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
    UPrimitiveComponent() = default;

    bool IsVisible()const;
    void SetVisible(bool newVisibility);

protected:
    void OnRegister() override;
    void OnUnregister() override;

    FPrimitiveSceneProxyHandle GetPrimitiveSceneProxy() const;

    void OnTransformUpdated() override;

private:
    bool isVisible = true;

    FPrimitiveSceneProxyHandle primitiveSceneProxyHandle;
};

}