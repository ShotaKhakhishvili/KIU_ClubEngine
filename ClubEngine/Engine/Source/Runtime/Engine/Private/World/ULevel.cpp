#include <Engine/World/ULevel.h>
#include <Engine/World/UWorld.h>

namespace CE
{

UWorld* ULevel::GetWorld() const
{
    return ownerWorld;
}

}