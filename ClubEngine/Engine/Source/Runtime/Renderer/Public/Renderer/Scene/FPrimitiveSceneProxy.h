#pragma once

#include <Core/ClubCore.h>
#include <Asset/UStaticMesh.h>

namespace CE
{

struct FPrimitiveSceneProxy
{
    TObjectHandle<UStaticMesh> staticMesh;
    FTransform transform;
};

}