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
    pathToHandle.clear();
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

void AssetRegistry::RegisterPath(const std::filesystem::path& path, const UObjectHandle handle)
{
    if (!IsAlive(handle))
        return;

    pathToHandle[NormalizePath(path)] = handle;
}

UObjectHandle AssetRegistry::FindHandleByPath(const std::filesystem::path& path) const
{
    const auto it = pathToHandle.find(NormalizePath(path));

    if (it == pathToHandle.end())
        return {};

    if (!registry.IsAlive(it->second))
    {
        pathToHandle.erase(it);
        return {};
    }

    return it->second;
}

std::string AssetRegistry::NormalizePath(const std::filesystem::path& path)
{
    std::error_code ec;
    std::filesystem::path normalized = std::filesystem::weakly_canonical(path, ec);

    if (ec)
        normalized = std::filesystem::absolute(path, ec);

    return normalized.lexically_normal().generic_string();
}

}