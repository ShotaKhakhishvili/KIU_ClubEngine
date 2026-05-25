#include <Core/Types/RHITypes.h>

#include <RHI.OpenGL/GLComputeShader.h>

#include "GLConvert.h"

#include <glad/glad.h>

namespace CE::RHI
{

GLComputeShader::GLComputeShader(const std::string& computeShaderSource)
    : GLShaderBase(ShaderDesc{ .stages = { ShaderSource{ShaderStage::Compute, computeShaderSource } } })
{

}

GLComputeShader::GLComputeShader(const std::filesystem::path& computeShaderFileSource)
    : GLShaderBase(ShaderFileDesc{ .stages = { ShaderFileSource{ShaderStage::Compute, computeShaderFileSource } } })
{

}

void GLComputeShader::Dispatch(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ)
{
    glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
}

}