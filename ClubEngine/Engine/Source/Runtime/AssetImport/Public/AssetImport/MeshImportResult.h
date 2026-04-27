#pragma once

#include <RenderCore/Vertex.h>

#include <cstdint>
#include <string>
#include <vector>

struct MeshImportSection
{
    std::string name = "Default";
    std::string materialName = "Default";

    uint32_t indexStart = 0;
    uint32_t indexCount = 0;
};

struct MeshImportResult
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshImportSection> sections;

    bool IsValid() const
    {
        return !vertices.empty() && !indices.empty();
    }
};