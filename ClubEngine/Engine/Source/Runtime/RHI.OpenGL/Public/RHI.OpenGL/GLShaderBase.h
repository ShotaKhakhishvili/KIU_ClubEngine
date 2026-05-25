#pragma once

#include <Core/Types/RHITypes.h>

#include <RHI.OpenGL/GLTexture.h>

#include <string>
#include <vector>
#include <unordered_map>

namespace CE::RHI
{

class GLShaderBase
{
public:
    GLShaderBase(const RHI::ShaderDesc& shaderDesc);
    GLShaderBase(const ShaderFileDesc& shaderFileDesc);
    ~GLShaderBase();

    void Bind() const;

    GLShaderBase(const GLShaderBase&) = delete;
    GLShaderBase& operator=(const GLShaderBase&) = delete;

    GLShaderBase(GLShaderBase&& other) noexcept;
    GLShaderBase& operator=(GLShaderBase&& other) noexcept;

    ShaderID GetID() const noexcept;

    void SetBool(const std::string& name, const bool    value);
    void SetInt(const std::string& name, const int32_t value);
    void SetFloat(const std::string& name, const float   value);
    void SetVec2(const std::string& name, const float   x, const float y);
    void SetVec3(const std::string& name, const float   x, const float y, const float z);
    void SetVec4(const std::string& name, const float   x, const float y, const float z, const float w);
    void SetIVec2(const std::string& name, const int   x, const int y);
    void SetIVec3(const std::string& name, const int   x, const int y, const int z);
    void SetIVec4(const std::string& name, const int   x, const int y, const int z, const int w);
    void SetTexture(const std::string& name, uint32_t slot);

    int32_t GetUniformLocation(const std::string& name);

private:

    ShaderID ID{ 0 };
    std::unordered_map<std::string, int32_t> uniformLocations;

    void Delete();
};

}