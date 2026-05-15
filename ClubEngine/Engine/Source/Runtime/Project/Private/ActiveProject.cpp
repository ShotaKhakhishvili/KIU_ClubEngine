#include <Project/ActiveProject.h>

namespace CE
{
    namespace
    {
        ProjectDescriptor activeProject;
    }

    void  SetActiveProject(const ProjectDescriptor& project)
    {
        activeProject = project;
    }
    const ProjectDescriptor& GetActiveProject()
    {
        return activeProject;
    }
}