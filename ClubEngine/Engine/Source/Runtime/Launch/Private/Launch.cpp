#include <Launch/Launch.h>
#include <Engine/Application.h>
#include <Engine/AssetLibrary.h>
#include <Asset/IAssetResolver.h>
#include <Core/ClubCore.h>

namespace CE
{
    int LaunchRun(Application* app)
    {
        SetCurrentAssetContext(&app->GetAssetContext());
        SetCurrentAssetResolver(&app->GetAssetContext());

        const int result = app->Run();

        SetCurrentAssetContext(nullptr);
        SetCurrentAssetResolver(nullptr);

        return result;
    }
}