#pragma once

#include <Core/Types/RHITypes.h>
#include <Core/Math/Matrix4x4.h>

#include <CoreUObject/UObject.h>

#include <RenderCore/RHI/RHIHandle.h>

#include <filesystem>
#include <memory>
#include <string>

namespace CE
{

class UShader final : public UObject
{
    GENERATED_BODY(UShader, UObject)

public:

    UShader(const RHI::ShaderDesc& shaderDesc, std::string inName = "UShader");
    ~UShader() override;

    UShader(const UShader&) = delete;
    UShader& operator=(const UShader&) = delete;
    UShader(UShader&&) noexcept = default;
    UShader& operator=(UShader&&) noexcept = default;

    bool Load();
    void Unload();
    bool IsReady() const noexcept;

    void Bind() const;
    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int32_t value);
    void SetFloat(const std::string& name, float value);
	void SetIVec2(const std::string& name, int x, int y);
	void SetIVec3(const std::string& name, int x, int y, int z);
	void SetIVec4(const std::string& name, int x, int y, int z, int w);
    void SetVec2(const std::string& name, float x, float y);
    void SetVec3(const std::string& name, float x, float y, float z);
    void SetVec4(const std::string& name, float x, float y, float z, float w);
    void SetMat4(const std::string& name, const FMatrix4x4F& value);

private:
    RHI::ShaderDesc shaderDesc;
    RHI::ShaderHandle handle;    
};

}