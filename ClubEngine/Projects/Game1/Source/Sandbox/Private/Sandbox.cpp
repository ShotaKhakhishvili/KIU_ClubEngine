#include <Sandbox.h>
#include <iostream>

int SandboxApp::Run()
{
    std::cout << "Sandbox running!\n";
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
