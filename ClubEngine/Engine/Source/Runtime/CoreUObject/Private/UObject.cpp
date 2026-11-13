#include <CoreUObject/UObject.h>

namespace CE
{

std::unordered_map<std::string, uint32> UObject::unnamedClassInstanceCounts;

UObjectHandle UObject::GetHandle() const noexcept
{
    return selfHandle;
}

void UObject::SetName(const char* inName)
{
    const std::string className = GetClass()->GetName();

    if (inName == nullptr)
    {
        const uint32 count = unnamedClassInstanceCounts[className]++;
        objectName = className + std::to_string(count);
    }
    else
    {
        objectName = className + std::string(":") + std::string(inName);
    }
}

uint64_t UObject::GetID() const noexcept { return id; }
const std::string& UObject::GetName() const noexcept { return objectName; }

UClass* UObject::StaticClass() noexcept
{
    static UClass classObject = UClass("UObject", nullptr, nullptr);
    return &classObject;
}

UClass* UObject::GetClass() const
{
    return StaticClass();
}
bool UObject::IsA(const UClass* Class) const noexcept
{
    return GetClass()->IsChildOf(Class);
}

}