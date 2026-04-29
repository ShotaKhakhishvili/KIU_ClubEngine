#include "CEConvert.h"

RHI::Backend ToRHI(CE::RenderBackend renderBackend)
{
    switch(renderBackend)
    {
        case CE::RenderBackend::OpenGL:
            return RHI::Backend::OpenGL;
        default:
            return RHI::Backend::OpenGL;
    }
}