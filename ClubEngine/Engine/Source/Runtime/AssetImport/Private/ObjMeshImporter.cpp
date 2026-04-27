#include <AssetImport/ObjMeshImporter.h>

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

struct ObjVertexKey
{
    int positionIndex = -1;
    int texCoordIndex = -1;
    int normalIndex = -1;

    bool operator==(const ObjVertexKey& other) const
    {
        return positionIndex == other.positionIndex &&
               texCoordIndex == other.texCoordIndex &&
               normalIndex == other.normalIndex;
    }
};

struct ObjVertexKeyHash
{
    std::size_t operator()(const ObjVertexKey& key) const
    {
        std::size_t h1 = std::hash<int>{}(key.positionIndex);
        std::size_t h2 = std::hash<int>{}(key.texCoordIndex);
        std::size_t h3 = std::hash<int>{}(key.normalIndex);

        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

ObjMeshImporter::ObjMeshImporter(ObjImportSettings inSettings)
    : settings(inSettings)
{
}

const std::vector<std::string>& ObjMeshImporter::GetErrors() const
{
    return errors;
}

const std::vector<std::string>& ObjMeshImporter::GetWarnings() const
{
    return warnings;
}

void ObjMeshImporter::ClearMessages()
{
    errors.clear();
    warnings.clear();
}

void ObjMeshImporter::AddError(std::string message)
{
    errors.push_back(std::move(message));
}

void ObjMeshImporter::AddWarning(std::string message)
{
    warnings.push_back(std::move(message));
}

static ObjVertexKey ParseFaceVertex(const std::string& text)
{
    ObjVertexKey key;

    std::stringstream ss(text);
    std::string part;

    if (std::getline(ss, part, '/') && !part.empty())
        key.positionIndex = std::stoi(part) - 1;

    if (std::getline(ss, part, '/') && !part.empty())
        key.texCoordIndex = std::stoi(part) - 1;

    if (std::getline(ss, part, '/') && !part.empty())
        key.normalIndex = std::stoi(part) - 1;

    return key;
}

static StaticMeshImportVertex BuildVertex(
    const ObjVertexKey& key,
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec2>& texCoords,
    const std::vector<glm::vec3>& normals,
    const ObjImportSettings& settings)
{
    StaticMeshImportVertex vertex;

    if (key.positionIndex >= 0 && key.positionIndex < static_cast<int>(positions.size()))
        vertex.position = positions[key.positionIndex] * settings.scale;

    if (key.texCoordIndex >= 0 && key.texCoordIndex < static_cast<int>(texCoords.size()))
    {
        vertex.texCoord = texCoords[key.texCoordIndex];

        if (settings.flipV)
            vertex.texCoord.y = 1.0f - vertex.texCoord.y;
    }

    if (key.normalIndex >= 0 && key.normalIndex < static_cast<int>(normals.size()))
        vertex.normal = normals[key.normalIndex];

    return vertex;
}

std::optional<StaticMeshImportData> ObjMeshImporter::Import(const std::filesystem::path& path)
{
    ClearMessages();

    std::ifstream file(path);

    if (!file.is_open())
    {
        AddError("Failed to open OBJ file: " + path.string());
        return std::nullopt;
    }

    StaticMeshImportData result;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::unordered_map<ObjVertexKey, uint32_t, ObjVertexKeyHash> vertexCache;

    StaticMeshImportSection currentSection;
    currentSection.name = "Default";
    currentSection.materialName = "Default";
    currentSection.indexStart = 0;

    auto FinishCurrentSection = [&]()
    {
        currentSection.indexCount =
            static_cast<uint32_t>(result.indices.size()) - currentSection.indexStart;

        if (currentSection.indexCount > 0)
            result.sections.push_back(currentSection);
    };

    std::string line;
    uint32_t lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;

        if (line.empty() || line[0] == '#')
            continue;

        std::stringstream ss(line);

        std::string type;
        ss >> type;

        if (type == "v")
        {
            glm::vec3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        }
        else if (type == "vt")
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            texCoords.push_back(uv);
        }
        else if (type == "vn")
        {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (type == "o" || type == "g")
        {
            FinishCurrentSection();

            currentSection = {};
            ss >> currentSection.name;

            if (currentSection.name.empty())
                currentSection.name = "Unnamed";

            currentSection.materialName = "Default";
            currentSection.indexStart = static_cast<uint32_t>(result.indices.size());
        }
        else if (type == "usemtl")
        {
            FinishCurrentSection();

            currentSection = {};
            currentSection.name = "Section";
            ss >> currentSection.materialName;

            if (currentSection.materialName.empty())
                currentSection.materialName = "Default";

            currentSection.indexStart = static_cast<uint32_t>(result.indices.size());
        }
        else if (type == "f")
        {
            std::vector<uint32_t> faceIndices;
            std::string vertexText;

            while (ss >> vertexText)
            {
                ObjVertexKey key;

                try
                {
                    key = ParseFaceVertex(vertexText);
                }
                catch (...)
                {
                    AddWarning("Invalid face vertex at line " + std::to_string(lineNumber));
                    continue;
                }

                auto it = vertexCache.find(key);

                if (it != vertexCache.end())
                {
                    faceIndices.push_back(it->second);
                }
                else
                {
                    StaticMeshImportVertex vertex =
                        BuildVertex(key, positions, texCoords, normals, settings);

                    uint32_t newIndex = static_cast<uint32_t>(result.vertices.size());

                    result.vertices.push_back(vertex);
                    vertexCache[key] = newIndex;
                    faceIndices.push_back(newIndex);
                }
            }

            if (faceIndices.size() < 3)
            {
                AddWarning("Face with less than 3 vertices at line " + std::to_string(lineNumber));
                continue;
            }

            if (!settings.triangulate && faceIndices.size() != 3)
            {
                AddWarning("Non-triangle face skipped at line " + std::to_string(lineNumber));
                continue;
            }

            for (size_t i = 1; i + 1 < faceIndices.size(); ++i)
            {
                result.indices.push_back(faceIndices[0]);
                result.indices.push_back(faceIndices[i]);
                result.indices.push_back(faceIndices[i + 1]);
            }
        }
    }

    FinishCurrentSection();

    if (!result.IsValid())
    {
        AddError("OBJ file did not contain valid mesh data: " + path.string());
        return std::nullopt;
    }

    return result;
}