#pragma once
#include <Core/Types/CoreTypes.h>

namespace CE
{
    struct FDelegateHandle
    {
        uint64 value = 0;

        bool IsValid()                         const;
        bool operator==(const FDelegateHandle&) const;
        bool operator!=(const FDelegateHandle&) const;

        static FDelegateHandle Generate();
        static FDelegateHandle Invalid();
        void Invalidate();
    };
}