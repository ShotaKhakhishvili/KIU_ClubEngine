#pragma once

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

    uint32_t index = std::numeric_limits<uint32_t>::max();
    uint32_t generation = 0;
};

}