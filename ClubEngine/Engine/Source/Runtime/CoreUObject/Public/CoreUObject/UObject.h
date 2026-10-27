#pragma once

#include <Core/ClubCore.h>
#include <CoreUObject/UObjectMacros.h> // This is used in every actor that extends UObject, so don't remove
#include <CoreUObject/UObjectHandle.h>

#include <string>
#include <unordered_map>

namespace CE
{

class UObject
{
public:
    virtual ~UObject() = default;

    UObject (const UObject&) = delete;
    UObject& operator= (const UObject&) = delete;
    UObject (UObject&&) = delete;
    UObject& operator= (UObject&&) = delete;

    uint64 GetID() const noexcept;
    const std::string& GetName() const noexcept;

    static const char* StaticClassName() noexcept;
    virtual const char* GetClassName() const noexcept;

    UObjectHandle GetHandle() const noexcept;

protected:
    explicit UObject(std::string_view inName = {}, std::string_view className = "UObject");

private:
    static std::unordered_map<std::string, uint32> unnamedClassInstanceCounts;

    friend class UObjectRegistry;

    UObjectHandle selfHandle;

    uint64 id = 0;
    std::string className;
    std::string objectName;
};

}