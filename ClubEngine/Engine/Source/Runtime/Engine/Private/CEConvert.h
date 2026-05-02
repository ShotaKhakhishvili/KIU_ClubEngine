#pragma once

#include <Core/Types/RHITypes.h>

#include <Engine/EngineTypes.h>

namespace CE
{

RHI::Backend ToRHI(CE::RenderBackend renderBackend);

}