#pragma once

#include <CoreUObject/UObjectRegistry.h>

#include <Asset/IAssetResolver.h>
#include <Asset/UShader.h>
#include <Asset/UTexture.h>
#include <Asset/UMaterial.h>
#include <Asset/UStaticMesh.h>

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

    void Destroy(UObjectHandle handle) const;

    UShader*            Resolve(TObjectHandle<UShader>          handle) const override;
    UTexture*           Resolve(TObjectHandle<UTexture>         handle) const override;
    UMaterial*          Resolve(TObjectHandle<UMaterial>        handle) const override;
    UStaticMesh*        Resolve(TObjectHandle<UStaticMesh>      handle) const override;

    TObjectHandle<UMaterial> GetDefaultMaterial() const;
    TObjectHandle<UShader> GetDefaultShader() const;
    TObjectHandle<UStaticMesh> GetDebugStaticMesh() const;

    template <typename T>
    T* Resolve(TObjectHandle<T> handle) const
    {
        return registry.Resolve(handle);
    }

private:
    friend class Application;

    void CreateEngineDefaults();

    mutable UObjectRegistry registry;

    TObjectHandle<UMaterial> defaultMaterial;
    TObjectHandle<UShader> defaultShader;
    TObjectHandle<UStaticMesh> debugStaticMesh;
};

}