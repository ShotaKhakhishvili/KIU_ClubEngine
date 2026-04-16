#pragma once

#include <Render/RenderTypes.h>
#include <string>
#include <map>

class Shader
{
public:
    Shader(const std::string& shaderName, const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void Bind() const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    ShaderID GetID() const noexcept;

    void SetBool    (const std::string& name, const bool    value);
    void SetInt     (const std::string& name, const int32_t value);
    void SetFloat   (const std::string& name, const float   value);
    void SetVec2    (const std::string& name, const float   x,      const float y);
    void SetVec3    (const std::string& name, const float   x,      const float y, const float z);
    void SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float a);

    int32_t GetUniformLocation(const std::string& name);

private:
    static std::map<std::string, int> globalShaderNames;
    std::string runtimeName;

    ShaderID ID{0};
    std::map<std::string, int32_t> uniformLocations;

    void Delete();
};
