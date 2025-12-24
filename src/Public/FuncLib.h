#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <stdexcept>
#include <limits>
#include <iostream>   // optional (for debug prints)

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // lookAt, perspective
#include <glm/gtc/quaternion.hpp>

struct Vertex
{
    glm::vec3 coord;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};

// --------------------------- Robust OBJ token parsing ---------------------------

struct ObjVertexToken {
    int v  = 0; // position index (1-based, can be negative)
    int vt = 0; // texcoord index (1-based), 0 if absent
    int vn = 0; // normal index (1-based), 0 if absent
};

inline bool TryParseInt(const std::string& s, int& out)
{
    // Empty means "missing"
    if (s.empty()) return false;

    // Accept leading +/- and digits only
    // (stoi can throw; we avoid exceptions by validating first)
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
    }

    // Now safe to stoi; still guard overflow
    try {
        long long val = std::stoll(s);
        if (val < std::numeric_limits<int>::min() || val > std::numeric_limits<int>::max())
            return false;
        out = (int)val;
        return true;
    } catch (...) {
        return false;
    }
}

// Parse one face token like:
// "453/528/491", "453//491", "453/528", "453", "-1/-2/-3"
inline bool TryParseOBJVertexToken(const std::string& token, ObjVertexToken& out)
{
    out = ObjVertexToken{};
    if (token.empty()) return false;

    // Split by '/'
    // We allow up to 3 fields: v, vt, vn
    // Some exporters may include extra slashes; we reject those tokens.
    int slashCount = 0;
    for (char c : token) if (c == '/') ++slashCount;
    if (slashCount > 2) return false;

    size_t p1 = token.find('/');
    if (p1 == std::string::npos) {
        // "v"
        return TryParseInt(token, out.v);
    }

    // v part
    if (p1 > 0) {
        if (!TryParseInt(token.substr(0, p1), out.v)) return false;
    } else {
        // missing v is invalid for faces
        return false;
    }

    size_t p2 = token.find('/', p1 + 1);
    if (p2 == std::string::npos) {
        // "v/vt"
        if (p1 + 1 < token.size()) {
            int tmp;
            if (TryParseInt(token.substr(p1 + 1), tmp)) out.vt = tmp;
            // if not parseable, treat as missing vt
        }
        return true;
    }

    // "v/vt/vn" or "v//vn"
    if (p2 > p1 + 1) {
        int tmp;
        if (TryParseInt(token.substr(p1 + 1, p2 - p1 - 1), tmp)) out.vt = tmp;
        // else treat as missing vt
    }
    if (p2 + 1 < token.size()) {
        int tmp;
        if (TryParseInt(token.substr(p2 + 1), tmp)) out.vn = tmp;
        // else treat as missing vn
    }

    return true;
}

// Resolve .obj index (1-based, can be negative) into 0-based index.
// Returns -1 if missing OR out of bounds.
inline int ResolveObjIndexSafe(int rawIndex, size_t containerSize)
{
    if (rawIndex == 0) return -1;
    if (containerSize == 0) return -1;

    long long idx = 0;
    if (rawIndex > 0) {
        idx = (long long)rawIndex - 1; // 1-based -> 0-based
    } else {
        // negative: -1 means "last", -2 second last, etc.
        idx = (long long)containerSize + (long long)rawIndex;
    }

    if (idx < 0 || idx >= (long long)containerSize) return -1;
    return (int)idx;
}

// --------------------------- Hash for vertex key ---------------------------

struct VertexKey {
    int p;
    int uv;
    int n;

    bool operator==(const VertexKey& other) const {
        return p == other.p && uv == other.uv && n == other.n;
    }
};

struct VertexKeyHash {
    std::size_t operator()(const VertexKey& k) const noexcept
    {
        // simple hash combine
        std::size_t h1 = std::hash<int>{}(k.p);
        std::size_t h2 = std::hash<int>{}(k.uv);
        std::size_t h3 = std::hash<int>{}(k.n);
        std::size_t h = h1;
        h ^= h2 + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        h ^= h3 + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        return h;
    }
};

// Reads an OBJ and fills out vertices and indices vectors.
// Robust behavior:
// - Skips malformed face tokens instead of crashing
// - Skips faces with invalid position indices instead of access-violating
// - UV/normal are optional
// - Triangulates polygons with fan
inline void ReadFromObjIntoVectors(
    const std::string& filename,
    std::vector<Vertex>& vertices,
    std::vector<GLuint>& indices,
    float positionScale = 0.1f, // default = divide by 10
    bool debugLog = false       // set true if you want skip logs
)
{
    std::ifstream f(filename);
    if (!f.is_open()) {
        throw std::runtime_error("Could not read an obj file: " + filename);
    }

    std::vector<glm::vec3> tmp_pos;
    std::vector<glm::vec2> tmp_uv;
    std::vector<glm::vec3> tmp_norm;

    std::unordered_map<VertexKey, GLuint, VertexKeyHash> vertexMap;
    vertexMap.reserve(4096);

    vertices.clear();
    indices.clear();

    std::string line;
    int lineNo = 0;

    while (std::getline(f, line)) {
        ++lineNo;
        if (line.empty()) continue;

        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        if (line[start] == '#') continue;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "v") {
            glm::vec3 p;
            ss >> p.x >> p.y >> p.z;
            tmp_pos.push_back(p);
        }
        else if (cmd == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            tmp_uv.push_back(uv);
        }
        else if (cmd == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            tmp_norm.push_back(n);
        }
        else if (cmd == "f") {
            std::vector<GLuint> faceVertexIndices;
            faceVertexIndices.reserve(8);

            std::string token;
            while (ss >> token) {
                ObjVertexToken t;
                if (!TryParseOBJVertexToken(token, t)) {
                    if (debugLog) {
                        std::cerr << "[OBJ] Skip bad face token at " << filename
                                  << ":" << lineNo << " token='" << token << "'\n";
                    }
                    continue;
                }

                int pIdx  = ResolveObjIndexSafe(t.v,  tmp_pos.size());
                if (pIdx == -1) {
                    if (debugLog) {
                        std::cerr << "[OBJ] Skip face vertex with invalid position index at "
                                  << filename << ":" << lineNo << " token='" << token
                                  << "' (posCount=" << tmp_pos.size() << ")\n";
                    }
                    continue;
                }

                int uvIdx = (t.vt != 0) ? ResolveObjIndexSafe(t.vt, tmp_uv.size()) : -1;
                int nIdx  = (t.vn != 0) ? ResolveObjIndexSafe(t.vn, tmp_norm.size()) : -1;

                VertexKey key{ pIdx, uvIdx, nIdx };

                auto it = vertexMap.find(key);
                if (it == vertexMap.end()) {
                    Vertex vert{};
                    vert.coord  = tmp_pos[pIdx] * positionScale;

                    // default values so shaders never read garbage
                    vert.texUV  = glm::vec2(0.0f, 0.0f);
                    vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                    vert.color  = glm::vec3(1.0f, 1.0f, 1.0f);

                    if (uvIdx != -1) vert.texUV = tmp_uv[uvIdx];
                    if (nIdx  != -1) vert.normal = tmp_norm[nIdx];

                    GLuint newIndex = (GLuint)vertices.size();
                    vertices.push_back(vert);
                    vertexMap.emplace(key, newIndex);
                    faceVertexIndices.push_back(newIndex);
                } else {
                    faceVertexIndices.push_back(it->second);
                }
            }

            // triangulate polygon (fan)
            if (faceVertexIndices.size() < 3) continue;
            for (size_t i = 1; i + 1 < faceVertexIndices.size(); ++i) {
                indices.push_back(faceVertexIndices[0]);
                indices.push_back(faceVertexIndices[i]);
                indices.push_back(faceVertexIndices[i + 1]);
            }
        }
        // ignore everything else
    }

    if (debugLog) {
        std::cerr << "[OBJ] Loaded '" << filename << "'"
                  << " pos=" << tmp_pos.size()
                  << " uv=" << tmp_uv.size()
                  << " n=" << tmp_norm.size()
                  << " finalVertices=" << vertices.size()
                  << " indices=" << indices.size() << "\n";
    }
}

