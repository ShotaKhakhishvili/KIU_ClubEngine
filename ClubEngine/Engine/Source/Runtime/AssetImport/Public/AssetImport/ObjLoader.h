#pragma once

#include <RenderCore/Vertex.h>

#include <string>
#include <vector>
#include <cstdint>

namespace Club::Render
{
    void LoadObjMesh(
        const std::string& filename,
        std::vector<Vertex>& vertices,
        std::vector<uint32_t>& indices,
        float positionScale = 0.1f,
        bool debugLog = false
    );
}