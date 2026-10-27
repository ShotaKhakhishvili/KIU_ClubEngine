#pragma once

#include <Core/Math/Vector.h>

#include <CoreUObject/TObjectHandle.h>

#include <Asset/UShader.h>
#include <Asset/UTexture.h>

#include <string>
#include <unordered_map>

namespace CE
{

struct FMaterialRenderState
{
    RHI::BlendMode blendMode = RHI::BlendMode::Opaque;
    RHI::CullMode cullMode = RHI::CullMode::Back;
    bool depthWrite = true;
    bool depthTest = true;
};

class UMaterial : public UObject
{
    GENERATED_BODY(UMaterial, UObject)

public:
    
    UMaterial(TObjectHandle<UShader> shader, std::string inName = "UMaterial");
    ~UMaterial();

    void SetBool    (const std::string& name, const bool    value);
    void SetInt     (const std::string& name, const int32_t value);
    void SetFloat   (const std::string& name, const float   value);
    void SetVec2    (const std::string& name, const float   x,      const float y);
    void SetVec3    (const std::string& name, const float   x,      const float y, const float z);
    void SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float w);
    void SetTexture (const std::string& name, TObjectHandle<UTexture>      texture);

    bool Bind() const;

    void Reset();

    const FMaterialRenderState& GetRenderState() const;
    void SetRenderState(const FMaterialRenderState& newRenderState);

private:
    TObjectHandle<UShader> shader;

    std::unordered_map<std::string, bool> bools;
    std::unordered_map<std::string, int32_t> ints;
    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, FVector2F> vectors_2;
    std::unordered_map<std::string, FVectorF> vectors_3;
    std::unordered_map<std::string, FVector4F> vectors_4;
    std::unordered_map<std::string, TObjectHandle<UTexture>> textures;

    FMaterialRenderState renderState;
};

}