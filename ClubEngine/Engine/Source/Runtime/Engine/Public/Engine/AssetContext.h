#pragma once

#include <Asset/IAssetResolver.h>
#include <Asset/UShader.h>
#include <Asset/UTexture.h>

#include <CoreUObject/UObjectRegistry.h>

#include <filesystem>
#include <utility>

namespace CE
{
    class AssetContext final : public IAssetResolver
    {
    public:
        AssetContext() = default;
        ~AssetContext() override = default;

        template<typename T, typename... Args>
        TObjectHandle<T> Create(Args&&... args)
        {
            return registry.Create<T>(std::forward<Args>(args)...);
        }

        TObjectHandle<UShader> CreateShader(
            std::filesystem::path vertexPath,
            std::filesystem::path fragmentPath,
            std::string name = "UShader"
        );

        TObjectHandle<UTexture> CreateTexture(
            std::filesystem::path sourcePath,
            UTexture::ImportSettings settings = UTexture::ImportSettings(),
            std::string name = "UTexture"
        );

        void Destroy(UObjectHandle handle);

        UShader* Resolve(TObjectHandle<UShader> handle) const override;
        UTexture* Resolve(TObjectHandle<UTexture> handle) const override;

    private:
        mutable UObjectRegistry registry;
    };
}