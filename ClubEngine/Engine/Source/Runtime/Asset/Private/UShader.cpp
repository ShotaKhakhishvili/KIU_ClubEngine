#include <Core/ClubCore.h>

#include <Asset/UShader.h>

#include <RenderCore/RHI/IRHI.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

namespace CE
{

UShader::UShader(const RHI::ShaderDesc& inShaderDesc, std::string inName)
	: UObject(std::move(inName)), shaderDesc(inShaderDesc)
{
}

UShader::~UShader()
{
	Unload();
}

const char* UShader::GetClassName() const noexcept
{
	return "UShader";
}

bool UShader::Load()
{
	Unload();

	handle = RHI::Get().CreateShader(shaderDesc);

	if(!handle.IsValid())
	{
		CE_LOG(Error, "Failed To Load Shader: [{}]", this->GetName());
		return false;
	}

	return true;
}

void UShader::Unload()
{
	if(!handle.IsValid())
		return;

	RHI::Get().DestroyShader(handle);
	handle.Invalidate();
}

bool UShader::IsReady() const noexcept
{
	return handle.IsValid();
}

void UShader::Bind() const
{
	if(!handle.IsValid())
		return;

	RHI::Get().BindShader(handle);
}

void UShader::SetBool(const std::string& name, bool value)
{
	if(!handle.IsValid())
		return;

	RHI::Get().BindShader(handle);
	RHI::Get().SetUniformBool(name, value);
}

void UShader::SetInt(const std::string& name, int32_t value)
{
	if(!handle.IsValid())
		return;

	RHI::Get().BindShader(handle);
	RHI::Get().SetUniformInt(name, value);
}

void UShader::SetFloat(const std::string& name, float value)
{
	if(!handle.IsValid())
		return;

	RHI::Get().BindShader(handle);
	RHI::Get().SetUniformFloat(name, value);
}

void UShader::SetVec2(const std::string& name, float x, float y)
{
	if(!handle.IsValid())
		return;

	RHI::Get().BindShader(handle);
	RHI::Get().SetUniformVec2(name, {x,y});
}

void UShader::SetVec3(const std::string& name, float x, float y, float z)
{
	if(!handle.IsValid())
		return;

	RHI::Get().BindShader(handle);
	RHI::Get().SetUniformVec3(name, {x,y,z});
}

void UShader::SetVec4(const std::string& name, float x, float y, float z, float w)
{
	if(!handle.IsValid())
		return;

	RHI::Get().BindShader(handle);
	RHI::Get().SetUniformVec4(name, {x,y,z,w});
}

}