#pragma once

#include <utility>

namespace CE
{

template<typename T, typename... Args>
TObjectHandle<T> AssetRegistry::Create(Args&&... args)
{
    return registry.Create<T>(std::forward<Args>(args)...);
}

template<typename T>
T* AssetRegistry::Resolve(TObjectHandle<T> handle) const
{
    return registry.Resolve(handle);
}

template<typename F>
void AssetRegistry::ForEach(F&& fn) const
{
    registry.ForEach(std::forward<F>(fn));
}

template<typename T, typename... Args>
TObjectHandle<T> NewAsset(Args&&... args)
{
    return GetAssetRegistry().Create<T>(std::forward<Args>(args)...);
}

template<typename T>
T* Resolve(TObjectHandle<T> handle)
{
    return GetAssetRegistry().Resolve(handle);
}

}