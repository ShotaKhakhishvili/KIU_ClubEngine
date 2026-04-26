#pragma once

#include <CoreUObject/TObjectHandle.h>

class UShader;
class UTexture;
class UMaterial;

class IAssetResolver
{
public:
    virtual ~IAssetResolver() = default;

    virtual UShader* Resolve(TObjectHandle<UShader> handle) const = 0;
    virtual UTexture* Resolve(TObjectHandle<UTexture> handle) const = 0;
    virtual UMaterial* Resolve(TObjectHandle<UMaterial> handle) const = 0;
};

namespace Asset
{
    IAssetResolver* GetCurrentAssetResolver() noexcept;
    void SetCurrentAssetResolver(IAssetResolver* resolver) noexcept;
}