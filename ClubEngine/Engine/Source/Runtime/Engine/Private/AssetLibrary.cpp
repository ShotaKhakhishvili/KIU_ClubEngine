#include <Core/ClubCore.h>

#include <Engine/AssetLibrary.h>
#include <Engine/AssetContext.h>


namespace CE
{

namespace
{
    AssetContext* GCurrentAssetContext = nullptr;
}

AssetContext* GetCurrentAssetContext() noexcept
{
    return GCurrentAssetContext;
}

const AssetContext* GetCurrentAssetContextConst() noexcept
{
    return GCurrentAssetContext;
}

void SetCurrentAssetContext(AssetContext* context) noexcept
{
    GCurrentAssetContext = context;
}

void DestroyAsset(UObjectHandle handle)
{
    if(GCurrentAssetContext == nullptr)
    {
        CE_LOG(Error, "DestroyAsset called without an active AssetContext");
        return;
    }

    GCurrentAssetContext->Destroy(handle);
}

}