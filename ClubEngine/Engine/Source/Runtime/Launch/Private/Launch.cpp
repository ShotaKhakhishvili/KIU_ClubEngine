#include <Launch/Launch.h>
#include <ClubEngine/Application.h>
#include <iostream>

namespace CE
{
    int LaunchRun(Application* app)
    {
        return app->Run();
    }
}