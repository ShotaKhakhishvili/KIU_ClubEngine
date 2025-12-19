#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <stdexcept>

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct Vertex
{
    glm::vec3 coord;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};

// helper: parse a single face token like "453/528/491", "453//491", "453/528" or "453"
struct ObjVertexToken {
    int v = 0;   // position index (1-based in .obj, can be negative)
    int vt = 0;  // texcoord index (1-based), 0 if absent
    int vn = 0;  // normal index (1-based), 0 if absent
};

inline ObjVertexToken ParseOBJVertexToken(const std::string& token)
{
    ObjVertexToken out{};
    size_t p1 = token.find('/');
    if (p1 == std::string::npos) {
        // only vertex index
        out.v = std::stoi(token);
        return out;
    }

    // vertex index (maybe before first slash)
    if (p1 > 0)
        out.v = std::stoi(token.substr(0, p1));

    size_t p2 = token.find('/', p1 + 1);
    if (p2 == std::string::npos) {
        // form: v/vt
        if (p1 + 1 < token.size())
            out.vt = std::stoi(token.substr(p1 + 1));
    }
    else {
        // form: v/vt/vn  OR  v//vn
        if (p2 > p1 + 1) // there is a vt between slashes
            out.vt = std::stoi(token.substr(p1 + 1, p2 - p1 - 1));

        if (p2 + 1 < token.size())
            out.vn = std::stoi(token.substr(p2 + 1));
    }

    return out;
}

// resolve .obj index (which is 1-based and can be negative) to 0-based index relative to container size
// returns -1 for "missing"
inline int ResolveObjIndex(int rawIndex, size_t containerSize)
{
    if (rawIndex > 0) return rawIndex - 1;      // 1-based -> 0-based
    if (rawIndex < 0) return (int)containerSize + rawIndex; // negative indexing
    return -1;
}

// Reads an OBJ and fills out vertices and indices vectors.
// - builds final vertices so each vertex stores the correct pos/uv/normal
// - reuses identical (pos,uv,norm) combos (no duplicate vertex entries for same triple)
inline void ReadFromObjIntoVectors(const std::string& filename, std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    std::ifstream f(filename);
    if (!f.is_open()) {
        throw std::runtime_error("Could not read an obj file: " + filename);
    }

    // temporary storage of raw .obj arrays
    std::vector<glm::vec3> tmp_pos;
    std::vector<glm::vec2> tmp_uv;
    std::vector<glm::vec3> tmp_norm;

    // map (posIdxResolved, uvIdxResolved, normIdxResolved) -> final vertex index
    std::map<std::tuple<int, int, int>, GLuint> vertexMap;

    vertices.clear();
    indices.clear();

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;

        // strip leading spaces
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        if (line[start] == '#') continue; // comment

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;
        if (cmd == "v") {
            glm::vec3 p; ss >> p.x >> p.y >> p.z;
            tmp_pos.push_back(p);
        }
        else if (cmd == "vt") {
            glm::vec2 uv; ss >> uv.x >> uv.y;
            tmp_uv.push_back(uv);
        }
        else if (cmd == "vn") {
            glm::vec3 n; ss >> n.x >> n.y >> n.z;
            tmp_norm.push_back(n);
        }
        else if (cmd == "f") {
            std::vector<GLuint> faceVertexIndices; // indices (into final vertices[]) for this face
            std::string token;
            while (ss >> token) {
                ObjVertexToken t = ParseOBJVertexToken(token);

                int pIdx = ResolveObjIndex(t.v, tmp_pos.size());
                int uvIdx = (t.vt != 0) ? ResolveObjIndex(t.vt, tmp_uv.size()) : -1;
                int nIdx = (t.vn != 0) ? ResolveObjIndex(t.vn, tmp_norm.size()) : -1;

                if (pIdx < 0 || pIdx >= (int)tmp_pos.size()) {
                    throw std::runtime_error("OBJ face references invalid position index in file: " + filename);
                }
                // uvIdx or nIdx may be -1 (absent) or out of range if the file is malformed; check gracefully:
                if (uvIdx < -1 || uvIdx >= (int)tmp_uv.size()) uvIdx = -1;
                if (nIdx < -1 || nIdx >= (int)tmp_norm.size()) nIdx = -1;

                auto key = std::make_tuple(pIdx, uvIdx, nIdx);

                auto it = vertexMap.find(key);
                if (it == vertexMap.end()) {
                    Vertex vert{};
                    vert.coord = tmp_pos[pIdx] / 10.0f;
                    if (uvIdx != -1) vert.texUV = tmp_uv[uvIdx];
                    if (nIdx != -1) vert.normal = tmp_norm[nIdx];

                    // ADD THIS LINE
                    vert.color = glm::vec3(1.0f, 1.0f, 1.0f);

                    GLuint newIndex = static_cast<GLuint>(vertices.size());
                    vertices.push_back(vert);
                    vertexMap.emplace(key, newIndex);
                    faceVertexIndices.push_back(newIndex);
                }

                else {
                    faceVertexIndices.push_back(it->second);
                }
            } // end while tokens of face

            // triangulate polygon (fan)
            if (faceVertexIndices.size() < 3) continue;
            for (size_t i = 1; i + 1 < faceVertexIndices.size(); ++i) {
                indices.push_back(faceVertexIndices[0]);
                indices.push_back(faceVertexIndices[i]);
                indices.push_back(faceVertexIndices[i + 1]);
            }
        }
        // ignore other commands (o, s, mtllib, usemtl, etc.)
    } // end file read
}