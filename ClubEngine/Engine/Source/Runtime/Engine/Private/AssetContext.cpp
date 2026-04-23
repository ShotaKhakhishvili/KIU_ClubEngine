#include <Engine/AssetContext.h>

#include <utility>

namespace CE
{
    TObjectHandle<UShader> AssetContext::CreateShader(
        std::filesystem::path vertexPath,
        std::filesystem::path fragmentPath,
        std::string name)
    {
        return registry.Create<UShader>(std::move(vertexPath), std::move(fragmentPath), std::move(name));
    }

    TObjectHandle<UTexture> AssetContext::CreateTexture(
        std::filesystem::path sourcePath,
        UTexture::ImportSettings settings,
        std::string name)
    {
        return registry.Create<UTexture>(std::move(sourcePath), settings, std::move(name));
    }

    void AssetContext::Destroy(UObjectHandle handle)
    {
        registry.Destroy(handle);
    }

    UShader* AssetContext::Resolve(TObjectHandle<UShader> handle) const
    {
        return registry.Resolve(handle);
    }

    UTexture* AssetContext::Resolve(TObjectHandle<UTexture> handle) const
    {
        return registry.Resolve(handle);
    }
}