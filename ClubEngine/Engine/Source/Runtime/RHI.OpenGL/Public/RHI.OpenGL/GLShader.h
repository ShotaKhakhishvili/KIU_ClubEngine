#pragma once

#include <Core/Types/RHITypes.h>

#include <RHI.OpenGL/GLTexture.h>

#include <string>
#include <unordered_map>

namespace CE::RHI
{

class GLShader
{
public:
    GLShader(const char* vertexPath, const char* fragmentPath);
    ~GLShader();

    void Bind() const;

    GLShader(const GLShader&) = delete;
    GLShader& operator=(const GLShader&) = delete;

    GLShader(GLShader&& other) noexcept;
    GLShader& operator=(GLShader&& other) noexcept;

    ShaderID GetID() const noexcept;

    void SetBool    (const std::string& name, const bool    value);
    void SetInt     (const std::string& name, const int32_t value);
    void SetFloat   (const std::string& name, const float   value);
    void SetVec2    (const std::string& name, const float   x,      const float y);
    void SetVec3    (const std::string& name, const float   x,      const float y, const float z);
    void SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float w);
    void SetTexture (const std::string& name, uint32_t slot);

    int32_t GetUniformLocation(const std::string& name);

private:

    ShaderID ID{0};
    std::unordered_map<std::string, int32_t> uniformLocations;

    void Delete();
};

}