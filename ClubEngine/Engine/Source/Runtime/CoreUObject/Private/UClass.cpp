#include <CoreUObject/UClass.h>

#include <cassert>
#include <CoreUObject/UObject.h>

namespace CE
{

namespace
{

std::unordered_map<std::string_view, UClass*>& GetClassMap()
{
    static std::unordered_map<std::string_view, UClass*> map;
    return map;
}

}


UClass::UClass(const char* name, UClass* superClass, const FClassConstructor constructor)
    : name(name), superClass(superClass), constructor(constructor)
{
    const bool inserted = GetClassMap().emplace(name, this).second;
    assert(inserted && "Duplicate UClass name");
}

const char* UClass::GetName() const noexcept
{
    return name;
}

UClass* UClass::GetSuperClass() const noexcept
{
    return superClass;
}

bool UClass::IsChildOf(const UClass* other) const noexcept
{
    if (other == nullptr)
        return false;

    for (const UClass* current = this; current != nullptr; current = current->superClass)
    {
        if (current == other)
            return true;
    }

    return false;
}

bool UClass::IsAbstract() const noexcept
{
    return constructor == nullptr;
}

UClass* UClass::FindClass(const std::string_view name)
{
    const auto it = GetClassMap().find(name);
    return it != GetClassMap().end() ? it->second : nullptr;
}

std::unique_ptr<UObject> UClass::Construct() const
{
    return constructor ? constructor() : nullptr;
}

}