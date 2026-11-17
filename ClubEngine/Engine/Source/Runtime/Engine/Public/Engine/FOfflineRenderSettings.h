#pragma once

#include <Core/Types/CoreTypes.h>

#include <filesystem>

namespace CE
{

struct FOfflineRenderSettings
{
    bool        enabled             = false;
    bool        forcedOpaqueAlpha   = true;
    float       fps                 = 30.0f;
    uint32      startFrame          = 0;
    uint32      endFrame            = 0;
    uint32      width               = 0;
    uint32      height              = 0;
    std::filesystem::path outputDir;

    float GetDeltaTime()  const;
    uint32   GetFrameCount() const;

    bool Validate() const;
};

}