#pragma once

#include <CoreUObject/UClass.h>

namespace CE
{

template<typename T>
class TSubclassOf
{
public:
    TSubclassOf() = default;
    TSubclassOf(UClass* inClass);

    template<typename U>
        requires std::is_base_of_v<T, U>
    TSubclassOf(const TSubclassOf<U>& other);

    UClass* Get() const;

    UClass* operator -> () const;
    UClass* operator *  () const;
    operator UClass*    () const;

private:
    UClass* classObject = nullptr;
};
}

#include "TSubclassOf.inl"