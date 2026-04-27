#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>

struct StaticMeshImportVertex
{
    glm::vec3 position {0.0f};
    glm::vec3 normal   {0.0f};
    glm::vec2 texCoord {0.0f};
};

struct StaticMeshImportSection
{
    std::string name;
    std::string materialName;

    uint32_t indexStart = 0;
    uint32_t indexCount = 0;
};

struct StaticMeshImportData
{
    std::vector<StaticMeshImportVertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<StaticMeshImportSection> sections;

    void Clear()
    {
        vertices.clear();
        indices.clear();
        sections.clear();
    }

    bool IsValid() const
    {
        return !vertices.empty() && !indices.empty();
    }
};