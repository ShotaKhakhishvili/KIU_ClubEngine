#include <Launch/Launch.h>
#include <Engine/Application.h>
#include <Engine/AssetLibrary.h>
#include <Asset/IAssetResolver.h>
#include <Core/ClubCore.h>
#include <iostream>

namespace CE
{
    int LaunchRun(Application* app)
    {
        SetCurrentAssetContext(&app->GetAssetContext());
        SetDefaultAssetResolver(&app->GetAssetContext());

        const int result = app->Run();

        SetCurrentAssetContext(nullptr);
        SetDefaultAssetResolver(nullptr);

        return result;
    }
}