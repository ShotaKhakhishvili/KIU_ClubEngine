#pragma once
#include <map>
#include <string>
#include <glm/glm.hpp>

struct Glyph
{
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class Font
{
public:
    bool Load(const std::string& path, unsigned int pixelSize);
    const Glyph& Get(char c) const;

private:
    std::map<char, Glyph> glyphs;
};
