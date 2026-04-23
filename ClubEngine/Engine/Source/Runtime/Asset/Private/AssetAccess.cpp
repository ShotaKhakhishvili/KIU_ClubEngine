#include <Asset/AssetAccess.h>
#include <Asset/IAssetResolver.h>

namespace CE
{
    UShader* ResolveShader(TObjectHandle<UShader> handle) noexcept
    {
        IAssetResolver* resolver = GetDefaultAssetResolver();

        if(resolver == nullptr)
            return nullptr;

        return resolver->Resolve(handle);
    }

    UTexture* ResolveTexture(TObjectHandle<UTexture> handle) noexcept
    {
        IAssetResolver* resolver = GetDefaultAssetResolver();

        if(resolver == nullptr)
            return nullptr;

        return resolver->Resolve(handle);
    }
}