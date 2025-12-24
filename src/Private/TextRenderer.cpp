#include "TextRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Path.h"
#include <iostream>

void TextRenderer::Init(int screenWidth, int screenHeight)
{
    shader = new Shader(
        Path::Shader("text.vert").c_str(),
        Path::Shader("text.frag").c_str()
    );

    shader->Activate();

    glm::mat4 projection = glm::ortho(
        0.0f, (float)screenWidth,
        0.0f, (float)screenHeight
    );

    glUniformMatrix4fv(
        glGetUniformLocation(shader->getID(), "projection"),
        1, GL_FALSE, &projection[0][0]
    );

    glUniform1i(
        glGetUniformLocation(shader->getID(), "text"),
        0
    );

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void TextRenderer::Draw(const Font& font,
                        const std::string& text,
                        float x,
                        float y,
                        float scale,
                        glm::vec3 color)
{
    if (!shader)
        return;

    shader->Activate();

    // Upload text color
    GLint colorLoc = glGetUniformLocation(shader->getID(), "textColor");
    glUniform3f(colorLoc, color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (char c : text)
    {
        const Glyph& g = font.Get(c);

        float xpos = x + g.bearing.x * scale;
        float ypos = y - (g.size.y - g.bearing.y) * scale;

        float w = g.size.x * scale;
        float h = g.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, g.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursor (FreeType uses 1/64 pixels)
        x += (g.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextRenderer::~TextRenderer()
{
    if (shader)
    {
        delete shader;
        shader = nullptr;
    }

    if (VBO)
        glDeleteBuffers(1, &VBO);

    if (VAO)
        glDeleteVertexArrays(1, &VAO);
}
