#pragma once
#include "Core/CommandLine.h"

#include <Launch/Launch.h>

#include <Project/FProjectDescriptor.h>
#include <Project/ActiveProject.h>

#include <Engine/Application.h>

namespace CE
{
    Application* CreateApplication(); // defined by the game
}

int main(int argc, char** argv)
{
    CE::CommandLine::Set(argc, argv);
    
    CE::FProjectDescriptor project;
    project.rootPath = CE_PROJECT_ROOT;

    CE::SetActiveProject(project);

    CE::Application* app = CE::CreateApplication();
    int code = CE::LaunchRun(app);
    delete app;
    return code;
}
