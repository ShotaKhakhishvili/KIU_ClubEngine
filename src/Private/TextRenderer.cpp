#include "TextRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Path.h"
#include <iostream>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern Font* FONT_MAIN;

TextRenderer::TextRenderer(const glm::vec2& position, float size, const glm::vec3& color, Font* font, const std::string& text, unsigned int width, unsigned int height)
{
    this->position = position;
    this->size = size;
    this->color = color;
    this->font = font;
    this->text = text;
    this->IsWidget = true;

    Init(width, height);
}

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
    : TextRenderer(glm::vec2(0, height - 36), 1,
                   glm::vec3(1,1,1),
                   FONT_MAIN,
                   "Text Renderer",
                   width, height)
{
}

TextRenderer::TextRenderer()
    : TextRenderer(WINDOW_WIDTH, WINDOW_HEIGHT)
{
}


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
        delete font;
        font = nullptr;
    }

    if (VBO)
        glDeleteBuffers(1, &VBO);

    if (VAO)
        glDeleteVertexArrays(1, &VAO);
}

void TextRenderer::SetPosition(const glm::vec2& position){this->position = position;}
void TextRenderer::SetSize(float size){this->size = size;}
void TextRenderer::SetColor(const glm::vec3& color){this->color = color;}
void TextRenderer::SetText(const std::string& text){this->text = text;}
void TextRenderer::SetFont(Font* font)
{
    this->font = font;
}

void TextRenderer::Draw()
{
    this->Draw(*font, text, position.x, position.y, size, color);
}
