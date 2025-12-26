#include "TextRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Path.h"
#include <iostream>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern Font* FONT_MAIN;

// ------------------------------------------------------------
// Static UI state
// ------------------------------------------------------------
glm::mat4 TextRenderer::projection  = glm::mat4(1.0f);
int       TextRenderer::screenWidth = 1;
int       TextRenderer::screenHeight = 1;

// ------------------------------------------------------------
// Ctors
// ------------------------------------------------------------
TextRenderer::TextRenderer(const glm::vec2& position,
                           float size,
                           const glm::vec3& color,
                           Font* font,
                           const std::string& text,
                           unsigned int width,
                           unsigned int height)
{
    this->position = position;
    this->size = size;
    this->color = color;
    this->font = font;
    this->text = text;
    this->IsWidget = true;

    Init((int)width, (int)height);
}

TextRenderer::TextRenderer(unsigned int width, unsigned int height)
    : TextRenderer(glm::vec2(20.0f, 20.0f), 1.0f,
                   glm::vec3(1, 1, 1),
                   FONT_MAIN,
                   "Text Renderer",
                   width, height)
{
}

TextRenderer::TextRenderer()
    : TextRenderer((unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT)
{
}

void TextRenderer::Init(int width, int height)
{
    // Set initial UI projection (top-left origin)
    OnResize(width, height);

    shader = new Shader(
        Path::Shader("text.vert").c_str(),
        Path::Shader("text.frag").c_str()
    );

    // VAO/VBO for 6 vertices, each has 4 floats: x,y,u,v
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 4, GL_FLOAT, GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void TextRenderer::OnResize(int width, int height)
{
    if (height == 0) height = 1;

    screenWidth  = width;
    screenHeight = height;

    // Bottom-left origin (OpenGL-native)
    // (0,0) bottom-left, y grows UP
    projection = glm::ortho(
        0.0f,
        static_cast<float>(width),
        0.0f,
        static_cast<float>(height)
    );
}



int TextRenderer::GetScreenWidth()  { return screenWidth; }
int TextRenderer::GetScreenHeight() { return screenHeight; }

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

    // Upload projection every draw (resize-safe)
    glUniformMatrix4fv(
        glGetUniformLocation(shader->getID(), "projection"),
        1, GL_FALSE,
        glm::value_ptr(TextRenderer::projection)
    );

    // Bind sampler explicitly
    glUniform1i(
        glGetUniformLocation(shader->getID(), "text"),
        0
    );

    // Text color
    glUniform3f(
        glGetUniformLocation(shader->getID(), "textColor"),
        color.x, color.y, color.z
    );

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (char c : text)
    {
        const Glyph& g = font.Get(c);

        float xpos = x + g.bearing.x * scale;

        // Standard FreeType baseline math (Y-up)
        float ypos = y + (g.bearing.y - g.size.y) * scale;

        float w = g.size.x * scale;
        float h = g.size.y * scale;

        // IMPORTANT: no flipping
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
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursor (FreeType uses 1/64 pixels)
        x += (g.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::Draw()
{
    if (!font) return;
    Draw(*font, text, position.x, position.y, size, color);
}

void TextRenderer::SetPosition(const glm::vec2& position) { this->position = position; }
void TextRenderer::SetSize(float size) { this->size = size; }
void TextRenderer::SetColor(const glm::vec3& color) { this->color = color; }
void TextRenderer::SetText(const std::string& text) { this->text = text; }
void TextRenderer::SetFont(Font* font) { this->font = font; }

TextRenderer::~TextRenderer()
{
    if (shader)
    {
        delete shader;
        shader = nullptr;
    }

    if (ownsFont && font)
    {
        delete font;
        font = nullptr;
    }
    else
    {
        font = nullptr;
    }

    if (VBO)
        glDeleteBuffers(1, &VBO);

    if (VAO)
        glDeleteVertexArrays(1, &VAO);

    VBO = 0;
    VAO = 0;
}
