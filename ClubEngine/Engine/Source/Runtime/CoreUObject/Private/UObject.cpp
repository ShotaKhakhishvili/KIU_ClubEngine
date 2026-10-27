#include <CoreUObject/UObject.h>

namespace CE
{

std::unordered_map<std::string, uint32> UObject::unnamedClassInstanceCounts;

UObject::UObject(std::string_view inObjectName, std::string_view inClassName)
{
    const uint32 count = unnamedClassInstanceCounts[className]++;
    className = inClassName;
    objectName = className + (inObjectName.empty() ? std::string(":") + std::string(inObjectName) : std::to_string(count));
}

const char* UObject::StaticClassName() noexcept { return "UObject"; }
const char* UObject::GetClassName() const noexcept { return "UObject"; }
UObjectHandle UObject::GetHandle() const noexcept
{
    return selfHandle;
}

uint64_t UObject::GetID() const noexcept { return id; }
const std::string& UObject::GetName() const noexcept { return objectName; }

}