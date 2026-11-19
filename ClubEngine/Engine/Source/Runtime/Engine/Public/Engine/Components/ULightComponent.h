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

    void SetLightParameters(const FLightParameters& newLightParameters);
    const FLightParameters& GetLightParameters() const;

    void SetLightColor(const FVectorF& newColor);
    const FVectorF& GetLightColor() const;

    void SetIntensity(float newIntensity);
    float GetIntensity() const;

    ELightType GetLightType() const;

protected:
    void OnTransformUpdated() override;
    void OnVisibilityChanged() override;

    void OnRegister() override;
    void OnUnregister() override;

    FLightParameters lightParameters;
    ELightType lightType = ELightType::Point;
    FLightSceneProxyHandle proxyHandle;
};

}
