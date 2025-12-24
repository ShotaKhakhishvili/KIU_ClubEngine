#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Font.h"
#include "Shader.h"

class TextRenderer
{
    unsigned int VAO = 0;
    unsigned int VBO = 0;

    Shader* shader = nullptr;

public:
    void Init(int screenWidth, int screenHeight);
    void Draw(const Font& font,
              const std::string& text,
              float x, float y,
              float scale,
              glm::vec3 color);
    ~TextRenderer();
};

