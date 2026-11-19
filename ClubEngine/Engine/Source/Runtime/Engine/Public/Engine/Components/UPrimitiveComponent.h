#pragma once

#include <Asset/UStaticMesh.h>

#include <Renderer/Scene/FScene.h>

#include <Engine/Components/USceneComponent.h>

namespace CE
{

class UPrimitiveComponent : public USceneComponent
{
    GENERATED_BODY(UPrimitiveComponent, USceneComponent)

public:
    UPrimitiveComponent() = default;

protected:
    void OnRegister() override;
    void OnUnregister() override;

    FPrimitiveSceneProxyHandle GetPrimitiveSceneProxy() const;

    void OnTransformUpdated() override;
    void OnVisibilityChanged() override;

private:
    FPrimitiveSceneProxyHandle primitiveSceneProxyHandle;
};

}