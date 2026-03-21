#pragma once
#include <Launch/Launch.h>
#include <Engine/Application.h>
#include <iostream>

namespace CE
{
    Application* CreateApplication(); // defined by the game
}

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    
    CE::Application* app = CE::CreateApplication();
    int code = CE::LaunchRun(app);
    delete app;
    return code;
}
