#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/TObjectHandle.h>

#include <Asset/UMaterial.h>
#include <Asset/UStaticMesh.h>

namespace CE
{

struct FPrimitiveSceneProxy
{
    TObjectHandle<UStaticMesh> staticMesh{};
    TArray<TObjectHandle<UMaterial>> overrideMaterials;
    FMatrix4x4F localToWorld = FMatrix4x4F::Identity();
    bool isVisible = true;
};

}