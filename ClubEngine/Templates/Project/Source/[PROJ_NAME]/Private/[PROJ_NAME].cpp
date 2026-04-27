#include <[PROJ_NAME].h>

#include <Core/ClubCore.h>

int [PROJ_NAME]App::Update(float dt)
{
    (void)GetAssetContext();
    (void)dt;

    return 0;
}

namespace CE
{
    Application* CreateApplication()
    {
        return new [PROJ_NAME]App();
    }
}
