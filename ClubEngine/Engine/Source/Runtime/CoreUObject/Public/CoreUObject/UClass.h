#pragma once

#include <memory>
#include <type_traits>

namespace CE
{

class UObject;
class UObjectRegistry;

using FClassConstructor = std::unique_ptr<UObject> (*)();

class UClass
{
public:
    UClass(const char* name, UClass* superClass, FClassConstructor constructor);

    UClass(const UClass&) = delete;
    UClass& operator=(const UClass&) = delete;
    UClass(UClass&&) = delete;
    UClass& operator=(UClass&&) = delete;

    const char* GetName() const noexcept;
    UClass* GetSuperClass() const noexcept;

    bool IsChildOf(const UClass* other) const noexcept;
    bool IsAbstract() const noexcept;

private:
    friend class UObjectRegistry;

    std::unique_ptr<UObject> Construct() const;

    const char* name;
    UClass* superClass;
    FClassConstructor constructor;
};

template<typename T>
constexpr FClassConstructor GetClassConstructor()
{
    if constexpr (std::is_abstract_v<T> || !std::is_default_constructible_v<T>)
        return nullptr;
    else
        return []() -> std::unique_ptr<UObject> { return std::make_unique<T>(); };
}

}