#pragma once

#include <Core/ClubCore.h>

#include <CoreUObject/UObjectMacros.h>

#include <string>
#include <cstdint>
#include <unordered_map>

namespace CE
{

class UObject
{
public:
    virtual ~UObject() = default;

    uint64_t GetID() const noexcept;
    const std::string& GetName() const noexcept;

    static const char* StaticClassName() noexcept;
    virtual const char* GetClassName() const noexcept;

protected:
    explicit UObject(std::string inName);
    explicit UObject(const char* className);

private:
    static std::unordered_map<std::string, uint32> unnamedClassInstanceCounts;

    friend class UObjectRegistry;

    uint64_t id = 0;
    std::string name;
};

}