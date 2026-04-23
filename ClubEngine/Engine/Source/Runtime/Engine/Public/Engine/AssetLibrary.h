#pragma once

#include <Engine/AssetContext.h>

#include <CoreUObject/TObjectHandle.h>
#include <CoreUObject/UObjectHandle.h>

#include <Core/ClubCore.h>

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
}