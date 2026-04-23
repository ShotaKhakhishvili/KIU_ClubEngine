#include <CoreUObject/UObject.h>

UObject::UObject(std::string inName)
    : name(std::move(inName))
    {
        
    }

uint64_t UObject::GetID() const noexcept { return id; }
const std::string& UObject::GetName() const noexcept { return name; }