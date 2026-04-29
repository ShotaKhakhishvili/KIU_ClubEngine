#include <RenderCore/RHI/IRHI.h>

#include <cassert>

namespace RHI
{
    static std::unique_ptr<IRHI> GCurrentRHI;

    IRHI& Get()
    {
        assert(GCurrentRHI);
        return *GCurrentRHI;
    }

    void Set(std::unique_ptr<IRHI> newRHI)
    {
        GCurrentRHI = std::move(newRHI);
    }
}