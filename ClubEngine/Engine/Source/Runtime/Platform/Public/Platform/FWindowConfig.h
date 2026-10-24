#pragma once

#include <Core/ClubCore.h>
#include <Core/Types/RHITypes.h>

#include <string>

namespace CE
{

struct FWindowConfig
{
    std::string     title       = "ClubeEngine";
    uint32          width       = 1280;
    uint32          height      = 720;
    bool            vSync       = true;
    RHI::Backend    backend     = RHI::Backend::OpenGL;
};

}