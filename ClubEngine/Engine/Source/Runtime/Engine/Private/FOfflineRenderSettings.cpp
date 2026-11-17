#include <Engine/FOfflineRenderSettings.h>
#include <Core/Log.h>

namespace CE
{

float FOfflineRenderSettings::GetDeltaTime() const
{
    return 1.0f / fps;
}

uint32 FOfflineRenderSettings::GetFrameCount() const
{
    return endFrame + 1;
}

bool FOfflineRenderSettings::Validate() const
{
    bool ok = true;

    if (fps <= 0.0f)
    {
        CE_LOG(Error, "OfflineRender: FPS must be > 0 (got {})", fps);
        ok = false;
    }
    if (endFrame < startFrame)
    {
        CE_LOG(Error, "OfflineRender: invalid frame range {}-{}", startFrame, endFrame);
        ok = false;
    }
    if (width == 0 || height == 0)
    {
        CE_LOG(Error, "OfflineRender: invalid output size {}x{}", width, height);
        ok = false;
    }
    if (outputDir.empty())
    {
        CE_LOG(Error, "OfflineRender: OutputDir is empty");
        ok = false;
    }

    return ok;
}

}