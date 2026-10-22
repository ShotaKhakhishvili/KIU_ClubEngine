#pragma once

#include <Core/Types/CoreTypes.h>

#include <cstdint>
#include <limits>

namespace CE
{

class UObjectHandle
{
public:
    bool IsValid() const noexcept;
    void Invalidate() noexcept;
    
private:
    friend class UObjectRegistry;

    uint32 index = std::numeric_limits<uint32_t>::max();
    uint32 generation = 0;
};

}