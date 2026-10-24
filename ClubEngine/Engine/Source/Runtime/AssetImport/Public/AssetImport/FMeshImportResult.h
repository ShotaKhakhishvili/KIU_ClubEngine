#pragma once

#include <Core/Types/FVertex.h>

#include <cstdint>
#include <string>
#include <vector>

namespace CE
{
    struct FMeshImportSection
    {
        std::string name = "Default";
        std::string materialName = "Default";

        uint32_t indexStart = 0;
        uint32_t indexCount = 0;
    };

    struct FMeshImportResult
    {
        std::vector<FVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<FMeshImportSection> sections;

        bool IsValid() const
        {
            return !vertices.empty() && !indices.empty();
        }
    };
}