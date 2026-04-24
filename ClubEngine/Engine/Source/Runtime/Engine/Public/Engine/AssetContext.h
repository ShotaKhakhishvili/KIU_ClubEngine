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

        void Destroy(UObjectHandle handle);

        UShader* Resolve(TObjectHandle<UShader> handle) const override;
        UTexture* Resolve(TObjectHandle<UTexture> handle) const override;

    private:
        mutable UObjectRegistry registry;
    };
}