#pragma once

#include <Asset/UStaticMesh.h>

#include <Engine/World/USceneComponent.h>

namespace CE
{

class UPrimitiveComponent : public USceneComponent
{
    GENERATED_BODY(UPrimitiveComponent, USceneComponent)

public:
    explicit UPrimitiveComponent(FObjectHandle<FPrimitiveSceneProxy> primitiveSceneProxy);

    bool IsVisible()const;
    void SetVisible(bool newVisibility);

private:
    bool isVisible = true;

    FObjectHandle<FPrimitiveSceneProxy> primitiveSceneProxy;
};

}