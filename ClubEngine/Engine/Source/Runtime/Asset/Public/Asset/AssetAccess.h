#pragma once

#include <CoreUObject/TObjectHandle.h>
#include <Asset/IAssetResolver.h>

class UShader;
class UTexture;

namespace CE
{
    template<typename T>
    T* Resolve(TObjectHandle<T> handle) noexcept
    {
        IAssetResolver* resolver = GetDefaultAssetResolver();

        if(resolver == nullptr)
            return nullptr;

        return resolver->Resolve(handle);  
    }
}