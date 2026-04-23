#include <CoreUObject/UObjectHandle.h>

bool UObjectHandle::IsValid() const noexcept 
{ 
    return this->index != std::numeric_limits<uint32_t>::max();    
}

void UObjectHandle::Invalidate() noexcept
{
    this->index = std::numeric_limits<uint32_t>::max();
    this->generation = 0;
}