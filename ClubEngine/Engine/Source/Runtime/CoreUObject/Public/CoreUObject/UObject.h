#pragma once

#include <string>
#include <cstdint>

class UObject
{
public:
    virtual ~UObject() = default;

    uint64_t GetID() const noexcept;
    const std::string& GetName() const noexcept;

    virtual const char* GetClassName() const noexcept = 0;

protected:
    explicit UObject(std::string inName);

private:
    friend class UObjectRegistry;

    uint64_t id = 0;
    std::string name;
};