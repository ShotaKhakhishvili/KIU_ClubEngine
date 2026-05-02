#include <Asset/IAssetResolver.h>

namespace CE
{
    namespace
    {
        IAssetResolver* GAssetResolver = nullptr;
    }

IAssetResolver* GetCurrentAssetResolver() noexcept
{
    return GAssetResolver;
}

void SetCurrentAssetResolver(IAssetResolver* resolver) noexcept
{
    GAssetResolver = resolver;
}

}