#pragma once

#include <CoreUObject/TObjectHandle.h>

namespace CE
{

class UShader;
class UTexture;
class UMaterial;
class UStaticMesh;

class IAssetResolver
{
public:
    virtual ~IAssetResolver() = default;

    virtual UShader* Resolve(TObjectHandle<UShader> handle) const = 0;
    virtual UTexture* Resolve(TObjectHandle<UTexture> handle) const = 0;
    virtual UMaterial* Resolve(TObjectHandle<UMaterial> handle) const = 0;
    virtual UStaticMesh* Resolve(TObjectHandle<UStaticMesh> handle) const = 0;
};

    IAssetResolver* GetCurrentAssetResolver() noexcept;
    void SetCurrentAssetResolver(IAssetResolver* resolver) noexcept;

}