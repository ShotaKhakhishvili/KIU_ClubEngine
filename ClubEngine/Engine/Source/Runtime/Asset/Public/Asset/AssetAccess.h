#pragma once

#include <CoreUObject/TObjectHandle.h>

class UShader;
class UTexture;

namespace CE
{
    UShader* ResolveShader(TObjectHandle<UShader> handle) noexcept;
    UTexture* ResolveTexture(TObjectHandle<UTexture> handle) noexcept;
}