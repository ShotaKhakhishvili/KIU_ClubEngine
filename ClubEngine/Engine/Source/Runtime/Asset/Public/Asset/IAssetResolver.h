#pragma once

#include <CoreUObject/TObjectHandle.h>

class UShader;
class UTexture;

class IAssetResolver
{
public:
    virtual ~IAssetResolver() = default;

    virtual UShader* Resolve(TObjectHandle<UShader> handle) const = 0;
    virtual UTexture* Resolve(TObjectHandle<UTexture> handle) const = 0;
};

IAssetResolver* GetDefaultAssetResolver() noexcept;
void SetDefaultAssetResolver(IAssetResolver* resolver) noexcept;