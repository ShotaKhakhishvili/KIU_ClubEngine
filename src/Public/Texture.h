#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>
#include "Shader.h"

class Texture
{
    GLenum ID;
    GLenum type;

    public:
    Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    Texture(){}

    void TexUnit(Shader& shader, const char* uniform, GLuint unit) const;
    void Bind() const;
    void Unbind() const;
    void Delete() const;

    GLuint getID() const;
    GLenum getType() const;
};