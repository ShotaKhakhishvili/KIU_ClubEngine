#pragma once

#include <Core/ClubCore.h>
#include <Core/Math/Transform.h>

#include <Engine/AssetContext.h>
#include <Engine/World/AActor.h>

#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObjectHandle.h>

#include <filesystem>
#include <string>
#include <utility>

namespace CE
{
class AssetContext;

AssetContext* GetCurrentAssetContext() noexcept;
const AssetContext* GetCurrentAssetContextConst() noexcept;
void SetCurrentAssetContext(AssetContext* context) noexcept;

template<typename T, typename... Args>
TObjectHandle<T> NewObject(Args&&... args)
{
    AssetContext* context = GetCurrentAssetContext();

    if(context == nullptr)
    {
        CE_LOG(Error, "NewObject called without an active AssetContext");
        return {};
    }

    return context->Create<T>(std::forward<Args>(args)...);
}

void DestroyAsset(UObjectHandle handle);

template<typename T>
T* SpawnActor(const FTransform& transform)
{
    static_assert(std::is_base_of_v<AActor, T>, "T must derive from AActor");

    T* actor = new T(transform);

    return actor;
}

}
