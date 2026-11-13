#include <CoreUObject/UClass.h>
#include <CoreUObject/UObject.h>

namespace CE
{

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

std::unique_ptr<UObject> UClass::Construct() const
{
    return constructor ? constructor() : nullptr;
}

}