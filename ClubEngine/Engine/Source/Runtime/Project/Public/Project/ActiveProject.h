#pragma once

#include <Project/ProjectDescriptor.h>

namespace CE
{
    void                     SetActiveProject(const ProjectDescriptor& project);
    const ProjectDescriptor& GetActiveProject();
}