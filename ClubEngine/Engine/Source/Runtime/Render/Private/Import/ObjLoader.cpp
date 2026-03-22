#include <Render/Import/ObjLoader.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Club::Render
{
    namespace
    {
        struct ObjVertexToken
        {
            int v  = 0;
            int vt = 0;
            int vn = 0;
        };

        bool TryParseInt(const std::string& s, int& out)
        {
            if (s.empty())
                return false;

            size_t i = 0;
            if (s[0] == '+' || s[0] == '-')
                i = 1;

            if (i >= s.size())
                return false;

            for (; i < s.size(); ++i)
            {
                if (s[i] < '0' || s[i] > '9')
                    return false;
            }

            try
            {
                const long long value = std::stoll(s);
                if (value < std::numeric_limits<int>::min() ||
                    value > std::numeric_limits<int>::max())
                {
                    return false;
                }

                out = static_cast<int>(value);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }

        bool TryParseObjVertexToken(const std::string& token, ObjVertexToken& out)
        {
            out = ObjVertexToken{};

            if (token.empty())
                return false;

            int slashCount = 0;
            for (char c : token)
            {
                if (c == '/')
                    ++slashCount;
            }

            if (slashCount > 2)
                return false;

            const size_t p1 = token.find('/');
            if (p1 == std::string::npos)
            {
                return TryParseInt(token, out.v);
            }

            if (p1 == 0)
                return false;

            if (!TryParseInt(token.substr(0, p1), out.v))
                return false;

            const size_t p2 = token.find('/', p1 + 1);
            if (p2 == std::string::npos)
            {
                if (p1 + 1 < token.size())
                {
                    int tmp = 0;
                    if (TryParseInt(token.substr(p1 + 1), tmp))
                        out.vt = tmp;
                }
                return true;
            }

            if (p2 > p1 + 1)
            {
                int tmp = 0;
                if (TryParseInt(token.substr(p1 + 1, p2 - p1 - 1), tmp))
                    out.vt = tmp;
            }

            if (p2 + 1 < token.size())
            {
                int tmp = 0;
                if (TryParseInt(token.substr(p2 + 1), tmp))
                    out.vn = tmp;
            }

            return true;
        }

        int ResolveObjIndexSafe(const int rawIndex, const size_t containerSize)
        {
            if (rawIndex == 0 || containerSize == 0)
                return -1;

            long long resolved = 0;

            if (rawIndex > 0)
            {
                resolved = static_cast<long long>(rawIndex) - 1;
            }
            else
            {
                resolved = static_cast<long long>(containerSize) +
                           static_cast<long long>(rawIndex);
            }

            if (resolved < 0 || resolved >= static_cast<long long>(containerSize))
                return -1;

            return static_cast<int>(resolved);
        }

        struct VertexKey
        {
            int PositionIndex = -1;
            int UVIndex       = -1;
            int NormalIndex   = -1;

            bool operator==(const VertexKey& other) const
            {
                return PositionIndex == other.PositionIndex &&
                       UVIndex == other.UVIndex &&
                       NormalIndex == other.NormalIndex;
            }
        };

        struct VertexKeyHash
        {
            std::size_t operator()(const VertexKey& key) const noexcept
            {
                std::size_t h1 = std::hash<int>{}(key.PositionIndex);
                std::size_t h2 = std::hash<int>{}(key.UVIndex);
                std::size_t h3 = std::hash<int>{}(key.NormalIndex);

                std::size_t h = h1;
                h ^= h2 + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
                h ^= h3 + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
                return h;
            }
        };
    }

    void LoadObjMesh(
        const std::string& filename,
        std::vector<Vertex>& vertices,
        std::vector<uint32_t>& indices,
        const float positionScale,
        const bool debugLog)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open OBJ file: " + filename);
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        std::unordered_map<VertexKey, uint32_t, VertexKeyHash> vertexMap;
        vertexMap.reserve(4096);

        vertices.clear();
        indices.clear();

        std::string line;
        int lineNo = 0;

        while (std::getline(file, line))
        {
            ++lineNo;

            if (line.empty())
                continue;

            const size_t start = line.find_first_not_of(" \t\r\n");
            if (start == std::string::npos)
                continue;

            if (line[start] == '#')
                continue;

            std::stringstream ss(line);
            std::string cmd;
            ss >> cmd;

            if (cmd == "v")
            {
                glm::vec3 p{};
                ss >> p.x >> p.y >> p.z;
                positions.push_back(p);
            }
            else if (cmd == "vt")
            {
                glm::vec2 uv{};
                ss >> uv.x >> uv.y;
                uvs.push_back(uv);
            }
            else if (cmd == "vn")
            {
                glm::vec3 n{};
                ss >> n.x >> n.y >> n.z;
                normals.push_back(n);
            }
            else if (cmd == "f")
            {
                std::vector<uint32_t> faceVertexIndices;
                faceVertexIndices.reserve(8);

                std::string token;
                while (ss >> token)
                {
                    ObjVertexToken parsedToken{};
                    if (!TryParseObjVertexToken(token, parsedToken))
                    {
                        if (debugLog)
                        {
                            std::cerr << "[OBJ] Skipping bad face token at "
                                      << filename << ":" << lineNo
                                      << " token='" << token << "'\n";
                        }
                        continue;
                    }

                    const int positionIndex =
                        ResolveObjIndexSafe(parsedToken.v, positions.size());

                    if (positionIndex == -1)
                    {
                        if (debugLog)
                        {
                            std::cerr << "[OBJ] Skipping face vertex with invalid position index at "
                                      << filename << ":" << lineNo
                                      << " token='" << token << "'\n";
                        }
                        continue;
                    }

                    const int uvIndex =
                        (parsedToken.vt != 0)
                            ? ResolveObjIndexSafe(parsedToken.vt, uvs.size())
                            : -1;

                    const int normalIndex =
                        (parsedToken.vn != 0)
                            ? ResolveObjIndexSafe(parsedToken.vn, normals.size())
                            : -1;

                    const VertexKey key{ positionIndex, uvIndex, normalIndex };

                    const auto it = vertexMap.find(key);
                    if (it == vertexMap.end())
                    {
                        Vertex vertex{};
                        vertex.coord  = positions[positionIndex] * positionScale;
                        vertex.texUV  = glm::vec2(0.0f, 0.0f);
                        vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                        vertex.color  = glm::vec3(1.0f, 1.0f, 1.0f);

                        if (uvIndex != -1)
                            vertex.texUV = uvs[uvIndex];

                        if (normalIndex != -1)
                            vertex.normal = normals[normalIndex];

                        const uint32_t newIndex =
                            static_cast<uint32_t>(vertices.size());

                        vertices.push_back(vertex);
                        vertexMap.emplace(key, newIndex);
                        faceVertexIndices.push_back(newIndex);
                    }
                    else
                    {
                        faceVertexIndices.push_back(it->second);
                    }
                }

                if (faceVertexIndices.size() < 3)
                    continue;

                for (size_t i = 1; i + 1 < faceVertexIndices.size(); ++i)
                {
                    indices.push_back(faceVertexIndices[0]);
                    indices.push_back(faceVertexIndices[i]);
                    indices.push_back(faceVertexIndices[i + 1]);
                }
            }
        }
    }
}