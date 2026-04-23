#include <Asset/IAssetResolver.h>

namespace
{
    IAssetResolver* GDefaultAssetResolver = nullptr;
}

IAssetResolver* GetDefaultAssetResolver() noexcept
{
    return GDefaultAssetResolver;
}

void SetDefaultAssetResolver(IAssetResolver* resolver) noexcept
{
    GDefaultAssetResolver = resolver;
}
