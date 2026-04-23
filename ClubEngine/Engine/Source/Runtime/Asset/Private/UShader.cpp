#include <Asset/UShader.h>

#include <RHI.OpenGL/Shader.h>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

UShader::UShader(std::filesystem::path inVertexPath,
				 std::filesystem::path inFragmentPath,
				 std::string inName)
	: UObject(std::move(inName))
	, fragmentPath(std::move(inFragmentPath))
	, vertexPath(std::move(inVertexPath))
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

	const std::string vertexPathString = vertexPath.string();
	const std::string fragmentPathString = fragmentPath.string();

	resource = std::make_unique<Shader>(vertexPathString.c_str(), fragmentPathString.c_str());

	if(resource == nullptr || resource->GetID() == 0)
	{
		resource.reset();
		return false;
	}

	return true;
}

void UShader::Unload()
{
	resource.reset();
}

bool UShader::IsReady() const noexcept
{
	return resource != nullptr && resource->GetID() != 0;
}

void UShader::Bind() const
{
	if(resource == nullptr)
		return;

	resource->Bind();
}

void UShader::SetBool(const std::string& name, bool value)
{
	if(resource == nullptr)
		return;

	resource->SetBool(name, value);
}

void UShader::SetInt(const std::string& name, int32_t value)
{
	if(resource == nullptr)
		return;

	resource->SetInt(name, value);
}

void UShader::SetFloat(const std::string& name, float value)
{
	if(resource == nullptr)
		return;

	resource->SetFloat(name, value);
}

void UShader::SetVec2(const std::string& name, float x, float y)
{
	if(resource == nullptr)
		return;

	resource->SetVec2(name, x, y);
}

void UShader::SetVec3(const std::string& name, float x, float y, float z)
{
	if(resource == nullptr)
		return;

	resource->SetVec3(name, x, y, z);
}

void UShader::SetVec4(const std::string& name, float x, float y, float z, float w)
{
	if(resource == nullptr)
		return;

	resource->SetVec4(name, x, y, z, w);
}

