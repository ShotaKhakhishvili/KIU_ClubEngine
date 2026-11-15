#pragma once

#include <Engine/World/USceneComponent.h>
#include <Engine/EngineTypes.h>

namespace CE
{

class UCameraComponent : public USceneComponent
{
    GENERATED_BODY(UCameraComponent, USceneComponent)

public:
    UCameraComponent() = default;

    FMinimalViewInfo GetCameraView() const;

protected:
    void OnRegister() override;
    void OnUnregister() override;

private:

    float fov = 90.0f;
    float zNear = 0.1f;
    float zFar = 10000.0f;

};

}
