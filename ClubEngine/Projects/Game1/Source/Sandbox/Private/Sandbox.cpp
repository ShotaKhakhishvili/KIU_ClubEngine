#include <Sandbox.h>

#include <Core/ClubCore.h>

int SandboxApp::Update(float dt)
{
    static float cnt = 0;
    
    GetAssetContext();

    (void)dt;

    cnt += dt;

    if(cnt >= 1.0f && this->FPSSystem != nullptr) // Log fps every second
    {
        CE_LOG(Info,  "FPS: {}", FPSSystem->GetFPS());
        cnt -= 1.0f;
    }
    
    return 0;
}

void SandboxApp::SetFpsSystem(FpsSystem* newFPSSystem)
{
    this->FPSSystem = newFPSSystem;
}

namespace CE
{
    Application* CreateApplication()
    {
        auto sandbox =  new SandboxApp();
        sandbox->SetFpsSystem(&(sandbox->AddSystem<FpsSystem>()));

        return sandbox;
    }
}
