#pragma once
#include <string>

class Path
{
public:
    // Roots
    static std::string ProjectRoot();
    static std::string Resources();

    // Typed subdirectories
    static std::string Shaders();
    static std::string Textures();
    static std::string Models();
    static std::string Fonts();

    // Generic helpers
    static std::string Asset(const std::string& relative);
    static std::string Shader(const std::string& relative);
    static std::string Texture(const std::string& relative);
    static std::string Model(const std::string& relative);
    static std::string Font(const std::string& relative);
};