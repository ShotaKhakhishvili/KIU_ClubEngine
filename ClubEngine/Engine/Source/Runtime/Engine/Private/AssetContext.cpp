#include <Engine/AssetContext.h>

#include <utility>

namespace CE
{
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

UMaterial* AssetContext::Resolve(TObjectHandle<UMaterial> handle) const
{
    return registry.Resolve(handle);
}

}