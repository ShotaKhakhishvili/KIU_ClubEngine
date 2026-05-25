#pragma once

#include <RHI.OpenGL/GLShaderBase.h>


namespace CE::RHI
{

class GLComputeShader : public GLShaderBase
{
public:
	GLComputeShader(const std::string& computeShaderPath);
	GLComputeShader(const std::filesystem::path& computeShaderFileSource);

	void Dispatch(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ);
};

}