#pragma once
#include <Launch/Launch.h>

#include <Project/ProjectDescriptor.h>
#include <Project/ActiveProject.h>

#include <Engine/Application.h>

namespace CE
{
    Application* CreateApplication(); // defined by the game
}

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    
    ProjectDescriptor project;
    project.rootPath = CE_PROJECT_ROOT;

    CE::SetActiveProject(project);

    CE::Application* app = CE::CreateApplication();
    int code = CE::LaunchRun(app);
    delete app;
    return code;
}
