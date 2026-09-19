#include <Engine/World/FObjectInitializer.h>
#include <Engine/World/ULevel.h>

namespace CE
{

namespace
{
    static inline thread_local ULevel* current;
}

FObjectInitializer::FObjectInitializer(ULevel* level) 
    : previous(current)
{
    current = level;
}

FObjectInitializer::~FObjectInitializer()
{
    current = previous;
}

ULevel* FObjectInitializer::GetCurrent()
{
    return current;
}

}