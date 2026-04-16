#include <Sandbox.h>
#include <Core/ClubCore.h>

int SandboxApp::Run()
{

    int slave = 42;
    CE_LOG(Warning,  "This is a formatted number: {}", slave);
    
    for(int i = 0; i > -1; i++){
        CE_LOG(Info, "Hello, Frame: {}", i);
    }
    return 0;
}

// int TheAnswerToTheUltimateQuestionOfLifeTheUniverseAndEverything()
// {
//     return 42;
// }

namespace CE
{
    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}
