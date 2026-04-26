#pragma once

#include <RenderCore/RHI/RHIHandle.h>
#include <RenderCore/RHI/RHITypes.h>

#include <cstdint>

struct SubMesh
{
    RHI::VertexArrayHandle vertexArray;
    RHI::DrawIndexedDesc drawDesc;

    uint32_t materialIndex = 0;
};