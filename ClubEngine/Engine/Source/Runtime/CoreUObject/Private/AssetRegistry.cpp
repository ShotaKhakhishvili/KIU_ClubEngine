#include <CoreUObject/AssetRegistry.h>

namespace CE
{

void AssetRegistry::Destroy(const UObjectHandle handle)
{
    registry.Destroy(handle);
}

bool AssetRegistry::IsAlive(const UObjectHandle handle) const
{
    return registry.IsAlive(handle);
}

void AssetRegistry::FlushPendingDestroy()
{
    registry.FlushPendingDestroy();
}

void AssetRegistry::Clear()
{
    registry.DestroyRegistry();
}

uint32 AssetRegistry::GetRegistryId() const
{
    return registry.GetRegistryId();
}

AssetRegistry& GetAssetRegistry() noexcept
{
    static AssetRegistry instance;
    return instance;
}

void DestroyAsset(const UObjectHandle handle)
{
    GetAssetRegistry().Destroy(handle);
}

}