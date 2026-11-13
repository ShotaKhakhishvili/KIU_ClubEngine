#pragma once

namespace CE
{

template <typename T>
TSubclassOf<T>::TSubclassOf(UClass* inClass)
    :   classObject(inClass)
{
}

template <typename T>
template<typename U>
    requires std::is_base_of_v<T, U>
TSubclassOf<T>::TSubclassOf(const TSubclassOf<U>& other)
    : classObject(other.Get())
{
}

template <typename T>
UClass* TSubclassOf<T>::Get() const
{
    return classObject && classObject->IsChildOf(T::StaticClass()) ? classObject : nullptr;
}

template <typename T>
UClass* TSubclassOf<T>::operator->()    const { return Get(); }
template <typename T>
UClass* TSubclassOf<T>::operator*()     const { return Get(); }
template <typename T>
TSubclassOf<T>::operator UClass*()      const { return Get(); }

}