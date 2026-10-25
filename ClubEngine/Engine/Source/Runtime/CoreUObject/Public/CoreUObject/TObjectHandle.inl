#pragma once
#include <CoreUObject/TObjectHandle.h>

namespace CE
{

template <typename T>
TObjectHandle<T>::TObjectHandle(UObjectHandle inHandle)
    :   handle(inHandle)
{}

template <typename T>
template<typename U>
    requires std::is_base_of_v<T, U>
TObjectHandle<T>::TObjectHandle(const TObjectHandle<U>& other) noexcept
    :   handle(other.GetRaw())
{}

template <typename T>
bool TObjectHandle<T>::IsValid() const noexcept
{
    return handle.IsValid();
}

template <typename T>
const UObjectHandle& TObjectHandle<T>::GetRaw() const noexcept
{
    return handle;
}

template <typename T>
bool TObjectHandle<T>::operator==(const TObjectHandle& other) const
{
    return this->GetRaw() == other.GetRaw();
}
template <typename T>
void TObjectHandle<T>::Invalidate()
{
    handle.Invalidate();
}
template <typename T>
uint32 TObjectHandle<T>::GetRegistryID() const noexcept
{
    return handle.GetRegistryID();
}

}