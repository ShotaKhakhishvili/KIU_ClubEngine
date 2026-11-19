#pragma once

#include <Renderer/Scene/FScene.h>
#include <Renderer/Scene/LightTypes.h>

#include <Engine/Components/USceneComponent.h>

namespace CE
{

class ULightComponent : public USceneComponent
{
    GENERATED_BODY(ULightComponent, USceneComponent)

public:
    ULightComponent() = default;

protected:
    void OnRegister() override;
    void OnUnregister() override;

    FLightSceneProxyHandle proxyHandle;
};

}
