#pragma once

#include <Project/FProjectDescriptor.h>

namespace CE
{
    void                     SetActiveProject(const FProjectDescriptor& project);
    const FProjectDescriptor& GetActiveProject();
}