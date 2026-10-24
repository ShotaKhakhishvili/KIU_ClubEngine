#include <Project/ActiveProject.h>

namespace CE
{
    namespace
    {
        FProjectDescriptor activeProject;
    }

    void  SetActiveProject(const FProjectDescriptor& project)
    {
        activeProject = project;
    }
    const FProjectDescriptor& GetActiveProject()
    {
        return activeProject;
    }
}