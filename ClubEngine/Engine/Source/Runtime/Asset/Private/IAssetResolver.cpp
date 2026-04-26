#include <Asset/IAssetResolver.h>

namespace
{
    IAssetResolver* GAssetResolver = nullptr;
}

namespace Asset
{
    IAssetResolver* GetCurrentAssetResolver() noexcept
    {
        return GAssetResolver;
    }

    void SetCurrentAssetResolver(IAssetResolver* resolver) noexcept
    {
        GAssetResolver = resolver;
    }
}