#pragma once

#include <Engine/Application.h>
#include <Engine/Systems/FpsSystem.h>

class SandboxApp : public CE::Application
{
public:
    int Update(float dt) override;

    void SetFpsSystem(FpsSystem* newFPSSystem);

private:
    FpsSystem* FPSSystem = nullptr;
};
