#include <RenderCore/RHI/IRHI.h>

#include <cassert>

namespace RHI
{
    static IRHI* GCurrentRHI = nullptr;

    IRHI& Get()
    {
        assert(GCurrentRHI);
        return *GCurrentRHI;
    }

    void Set(IRHI* rhi)
    {
        GCurrentRHI = rhi;
    }
}