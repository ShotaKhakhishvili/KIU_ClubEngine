#pragma once

#include <Core/Types/RenderTypes.h>
#include <Core/Math/Vector.h>

#include <CoreUObject/TObjectHandle.h>

#include <Asset/UShader.h>
#include <Asset/UTexture.h>

#include <string>
#include <unordered_map>

class UMaterial : public UObject
{
public:
    const char* GetClassName() const noexcept override;
    
    UMaterial(TObjectHandle<UShader> shader);
    ~UMaterial();

    UMaterial& operator=(const UMaterial& other) = delete;
    UMaterial(const UMaterial& other) = delete;

    UMaterial& operator=(UMaterial&& other) noexcept;
    UMaterial(UMaterial&& other) noexcept;

    void SetBool    (const std::string& name, const bool    value);
    void SetInt     (const std::string& name, const int32_t value);
    void SetFloat   (const std::string& name, const float   value);
    void SetVec2    (const std::string& name, const float   x,      const float y);
    void SetVec3    (const std::string& name, const float   x,      const float y, const float z);
    void SetVec4    (const std::string& name, const float   x,      const float y, const float z, const float w);
    void SetTexture (const std::string& name, TObjectHandle<UTexture>      texture);

    void Bind() const;

    void Reset();

private:
    TObjectHandle<UShader> shader;

    std::unordered_map<std::string, bool> bools;
    std::unordered_map<std::string, int32_t> ints;
    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, Vec2f> vectors_2;
    std::unordered_map<std::string, Vec3f> vectors_3;
    std::unordered_map<std::string, Vec4f> vectors_4;
    std::unordered_map<std::string, TObjectHandle<UTexture>> textures;

    RHI::BlendMode blendMode = RHI::BlendMode::Opaque;
    RHI::CullMode cullMode = RHI::CullMode::Back;
    bool depthWrite = true;
    bool depthTest = true;
};