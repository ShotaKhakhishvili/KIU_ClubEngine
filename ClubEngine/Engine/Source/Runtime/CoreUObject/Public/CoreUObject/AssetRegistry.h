#pragma once

#include <Core/Containers/TMap.h>

#include <CoreUObject/UObjectRegistry.h>
#include <CoreUObject/UObjectHandle.h>
#include <CoreUObject/TObjectHandle.h>

#include <filesystem>
#include <string>
namespace CE
{

class AssetRegistry
{
public:
    AssetRegistry() = default;

    AssetRegistry(const AssetRegistry&) = delete;
    AssetRegistry& operator=(const AssetRegistry&) = delete;

    template<typename T, typename... Args>
    TObjectHandle<T> Create(Args&&... args);

    template<typename T>
    T* Resolve(TObjectHandle<T> handle) const;

    template<typename F>
    void ForEach(F&& fn) const;

    void Destroy(UObjectHandle handle);
    bool IsAlive(UObjectHandle handle) const;

    void FlushPendingDestroy();
    void Clear();

    uint32 GetRegistryId() const;

    void RegisterPath(const std::filesystem::path& path, UObjectHandle handle);

    template<typename T>
    TObjectHandle<T> FindByPath(const std::filesystem::path& path) const;

private:
    UObjectHandle FindHandleByPath(const std::filesystem::path& path) const;
    static std::string NormalizePath(const std::filesystem::path& path);

    mutable std::unordered_map<std::string, UObjectHandle> pathToHandle;

    mutable UObjectRegistry registry;
};

AssetRegistry& GetAssetRegistry() noexcept;

template<typename T, typename... Args>
TObjectHandle<T> NewAsset(Args&&... args);

template<typename T>
T* Resolve(TObjectHandle<T> handle);

void DestroyAsset(UObjectHandle handle);

}

#include "AssetRegistry.inl"