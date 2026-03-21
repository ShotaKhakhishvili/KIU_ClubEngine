#include <Launch/Launch.h>
#include <Engine/Application.h>
#include <Core/Log.h>
#include <iostream>

namespace CE
{
    int LaunchRun(Application* app)
    {
        return app->Run();
    }
}