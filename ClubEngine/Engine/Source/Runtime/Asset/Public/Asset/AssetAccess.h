#pragma once

#include <CoreUObject/TObjectHandle.h>
#include <Asset/IAssetResolver.h>

namespace CE
{

class UShader;
class UTexture;

template<typename T>
T* Resolve(TObjectHandle<T> handle) noexcept
{
    IAssetResolver* resolver = GetCurrentAssetResolver();

    if(resolver == nullptr)
        return nullptr;

    return resolver->Resolve(handle);  
}

}