#pragma once

#include <Core/Types/CoreTypes.h>
#include <Core/Types/RHITypes.h>

#include <RenderCore/RHI/RHIHandle.h>

#include <cstdint>

namespace CE
{

struct FSubMesh
{
    RHI::VertexArrayHandle vertexArray;
    RHI::DrawIndexedDesc drawDesc;

    uint32 materialIndex = 0;
};

}