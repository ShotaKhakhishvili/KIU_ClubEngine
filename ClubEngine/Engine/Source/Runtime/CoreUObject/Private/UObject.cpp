#include <CoreUObject/UObject.h>

namespace CE
{

std::unordered_map<std::string, uint32> UObject::unnamedClassInstanceCounts;

const char* UObject::StaticClassName() noexcept { return "UObject"; }
const char* UObject::GetClassName() const noexcept { return "UObject"; }

UObjectHandle UObject::GetHandle() const noexcept
{
    return selfHandle;
}

void UObject::SetName(const char* inName)
{
    if (inName == nullptr)
    {
        const uint32 count = unnamedClassInstanceCounts[GetClassName()]++;
        objectName = GetClassName() + std::to_string(count);
    }
    else
    {
        objectName = GetClassName() + std::string(":") + std::string(inName);
    }
}

uint64_t UObject::GetID() const noexcept { return id; }
const std::string& UObject::GetName() const noexcept { return objectName; }

}