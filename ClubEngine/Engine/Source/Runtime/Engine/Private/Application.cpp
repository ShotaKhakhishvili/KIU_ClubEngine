#include <Engine/Application.h>

namespace CE
{
    Application::Application() = default;
    Application::~Application() = default;

    AssetContext& Application::GetAssetContext() noexcept
    {
        return assetContext;
    }

    const AssetContext& Application::GetAssetContext() const noexcept
    {
        return assetContext;
    }
}
