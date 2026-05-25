#include <Core/ClubCore.h>

#include "GLConvert.h"
#include <RHI.OpenGL/GLShaderBase.h>

#include <glad/glad.h>

#include <string>

namespace CE::RHI
{


namespace ShaderUtils
{

    bool CheckCompileErrors(GLuint shader, bool shaderProgram)
    {
        GLint success = GL_FALSE;
        char infoLog[512];

        if (!shaderProgram)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                CE_LOG(Error, "[Shader][Type : {}] SHADER ERROR:\n{}", shaderProgram, infoLog);
                return false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (success == GL_FALSE)
            {
                glGetProgramInfoLog(shader, 512, nullptr, infoLog);
                CE_LOG(Error, "[Shader][Type : {}] PROGRAM LINK ERROR:\n{}", shaderProgram, infoLog);
                return false;
            }
        }

        return true;
    }

    GLuint CompileShader(const char* source, ShaderStage stage, bool shaderProgram)
    {
        GLuint shader = glCreateShader(ToGL(stage));
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        if (!CheckCompileErrors(shader, shaderProgram))
        {
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLuint LinkProgram(std::vector<GLuint> shaders)
    {
        GLuint programID = glCreateProgram();
        for (auto shader : shaders)
        {
            glAttachShader(programID, shader);
        }
        glLinkProgram(programID);

        if (!CheckCompileErrors(programID, true))
        {
            glDeleteProgram(programID);
            return 0;
        }

        return programID;
    }
}

GLShaderBase::GLShaderBase(const ShaderFileDesc& shaderFileDesc)
{
    ShaderDesc shaderDesc = { .stages{shaderFileDesc.stages.size() } };

	for (uint32 i = 0; i < shaderFileDesc.stages.size(); i++)
    {
        shaderDesc.stages[i] = ShaderSource{ shaderFileDesc.stages[i].stage, CE::FileIO::ReadFile(shaderFileDesc.stages[i].source) };
    }

	*this = GLShaderBase(shaderDesc);
}

GLShaderBase::GLShaderBase(const ShaderDesc& shaderDesc)
{
    std::vector<GLuint> shaders;

    for (auto stage : shaderDesc.stages)
    {
        const GLuint shader = ShaderUtils::CompileShader(stage.source.c_str(), stage.stage, false);

        shaders.push_back(shader);
    }

    ID = ShaderUtils::LinkProgram(shaders);

    for (auto shader : shaders)
    {
        glDeleteShader(shader);
    }
}

void GLShaderBase::Bind() const
{
    glUseProgram(ID);
}

void GLShaderBase::Delete()
{
    if (ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0;
    }
}

ShaderID GLShaderBase::GetID() const noexcept
{
    return ID;
}

GLShaderBase::~GLShaderBase()
{
    Delete();
}

GLShaderBase::GLShaderBase(GLShaderBase&& other) noexcept
    : ID(other.ID)
{
    this->uniformLocations = std::move(other.uniformLocations);

    other.uniformLocations.clear();

    other.ID = 0;
}

GLShaderBase& GLShaderBase::operator=(GLShaderBase&& other) noexcept
{
    if (this != &other)
    {
        Delete();

        this->uniformLocations = std::move(other.uniformLocations);

        other.uniformLocations.clear();

        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

int32_t GLShaderBase::GetUniformLocation(const std::string& name)
{
    auto it = uniformLocations.find(name);
    if(it != uniformLocations.end())
        return it->second;

    const int32_t loc = glGetUniformLocation(ID, name.c_str());
    if(loc == -1){
        CE_LOG(Warning, "Uniform \"{}\" not found in GLShaderBase", name); 
    }

    uniformLocations[name] = loc;

    return loc;
}

void GLShaderBase::SetFloat(const std::string& name, const float value)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform1f(location, value);
}

void GLShaderBase::SetInt(const std::string& name, const int32_t value)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform1i(location, value);
}

void GLShaderBase::SetBool(const std::string& name, const bool value)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform1i(location, value ? 1 : 0);
}

void GLShaderBase::SetVec2(const std::string& name, const float x, const float y)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform2f(location, x,y);
}

void GLShaderBase::SetVec3(const std::string& name, const float x, const float y, const float z)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform3f(location, x,y,z);
}

void GLShaderBase::SetVec4(const std::string& name, const float x, const float y, const float z, const float w)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform4f(location, x,y,z,w);
}

void GLShaderBase::SetIVec2(const std::string& name, const int x, const int y)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform2i(location, x, y);
}

void GLShaderBase::SetIVec3(const std::string& name, const int x, const int y, const int z)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform3i(location, x, y, z);
}

void GLShaderBase::SetIVec4(const std::string& name, const int x, const int y, const int z, const int w)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform4i(location, x, y, z, w);
}

void GLShaderBase::SetTexture(const std::string& name, uint32_t slot)
{
    const int32_t location = GetUniformLocation(name);
    if (location == -1)
        return;

    glUniform1i(location, slot);
}

}