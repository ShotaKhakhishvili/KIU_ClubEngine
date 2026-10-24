#include "CEConvert.h"

namespace CE
{

RHI::Backend ToRHI(CE::ERenderBackend renderBackend)
{
    switch(renderBackend)
    {
        case CE::ERenderBackend::OpenGL:
            return RHI::Backend::OpenGL;
        default:
            return RHI::Backend::OpenGL;
    }
}

}