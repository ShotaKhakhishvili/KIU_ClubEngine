#include <Sandbox.h>
#include <Core/ClubCore.h>

int SandboxApp::Run()
{
    CE_LOG("ClubEngine SandboxApp is running!, printed using the logger by the way");
    
    for(int i = 0; i > -1; i++){
    }
    return 0;
}

namespace CE
{
    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}
