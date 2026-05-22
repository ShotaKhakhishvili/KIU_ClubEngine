#include <CoreUObject/UObject.h>

namespace CE
{

std::unordered_map<std::string, uint32> UObject::unnamedClassInstanceCounts;

UObject::UObject(const char* className)
{
    const uint32 count = unnamedClassInstanceCounts[className]++;
    name = className + std::to_string(count);
}

UObject::UObject(std::string inName)
    : name(std::move(inName))
{
}

const char* UObject::StaticClassName() noexcept { return "UObject"; }
const char* UObject::GetClassName() const noexcept { return "UObject"; }

uint64_t UObject::GetID() const noexcept { return id; }
const std::string& UObject::GetName() const noexcept { return name; }

}