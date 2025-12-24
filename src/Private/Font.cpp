#include "Font.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <iostream>

bool Font::Load(const std::string& path, unsigned int pixelSize)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "FreeType init failed\n";
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, path.c_str(), 0, &face))
    {
        std::cout << "Failed to load font\n";
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, pixelSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 32; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        unsigned int tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glyphs[c] = {
            tex,
            { face->glyph->bitmap.width, face->glyph->bitmap.rows },
            { face->glyph->bitmap_left, face->glyph->bitmap_top },
            (unsigned int)face->glyph->advance.x
        };
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return true;
}

const Glyph& Font::Get(char c) const
{
    static Glyph empty{};

    auto it = glyphs.find(c);
    if (it == glyphs.end())
        return empty;

    return it->second;
}

Font::Font(const std::string& path, unsigned int pixelSize)
{
    if (!Load(path, pixelSize))
    {
        std::cout << "Font constructor failed to load: " << path << std::endl;
    }
}