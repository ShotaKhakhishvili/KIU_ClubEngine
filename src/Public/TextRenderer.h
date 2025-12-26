#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Font.h"
#include "Shader.h"
#include "Actor.h"

#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Font.h"
#include "Shader.h"
#include "Actor.h"

class TextRenderer : public Actor
{
    unsigned int VAO = 0;
    unsigned int VBO = 0;

    Shader* shader = nullptr;
    bool ownsFont = false;

    Font* font = nullptr;
    glm::vec2 position;
    glm::vec3 color;
    std::string text;
    float size;

    // ----------------------------
    // STATIC UI STATE (NEW)
    // ----------------------------
    static glm::mat4 projection;
    static int screenWidth;
    static int screenHeight;

public:
    // ----------------------------
    // STATIC API (NEW)
    // ----------------------------
    static void OnResize(int width, int height);
    static int  GetScreenWidth();
    static int  GetScreenHeight();

    // ----------------------------
    // NORMAL API
    // ----------------------------
    TextRenderer(const glm::vec2& position, float size,
                 const glm::vec3& color, Font* font,
                 const std::string& text,
                 unsigned int width, unsigned int height);

    TextRenderer(unsigned int width, unsigned int height);
    TextRenderer();

    void Init(int screenWidth, int screenHeight);

    void Draw(const Font& font,
              const std::string& text,
              float x, float y,
              float scale,
              glm::vec3 color);

    void Draw() override;

    void SetPosition(const glm::vec2& position);
    void SetSize(float size);
    void SetColor(const glm::vec3& color);
    void SetText(const std::string& text);
    void SetFont(Font* font);

    ~TextRenderer();
};



