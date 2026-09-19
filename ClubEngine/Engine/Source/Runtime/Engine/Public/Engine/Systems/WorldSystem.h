#pragma once

#include <Engine/ISystem.h>
#include <Engine/World/UWorld.h>

namespace CE
{

class WorldSystem : public ISystem
{
public:
    const char*         GetName     ()              const       noexcept        override;
    bool                Init        ()                                          override;
    int                 Update      (float dt)                                  override;

    const UWorld&       GetWorld    ()              const                               ;
    UWorld&             GetWorld    ()                                                  ;

private:
    UWorld world;
};

}