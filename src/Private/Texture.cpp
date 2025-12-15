#include "Texture.h"
#include "Path.h"

Texture::Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = texType;

    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(Path::Texture(imagePath).c_str(), &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    // Resolution filtering
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Wrapping
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(texType);

    stbi_image_free(bytes);
    glBindTexture(texType, 0);
}

void Texture::TexUnit(Shader shader, const char* uniform, GLuint unit) const
{
    GLuint tex0Loc = glGetUniformLocation(shader.getID(), uniform);
    shader.Activate();
    glUniform1i(tex0Loc, unit);
}
void Texture::Bind() const
{
    glBindTexture(type, ID);
}
void Texture::Unbind() const
{
    glBindTexture(type, 0);
}
void Texture::Delete() const
{
    glDeleteTextures(1, &ID);
}

GLuint Texture::getID() const{return ID;}
GLenum Texture::getType() const{return type;}