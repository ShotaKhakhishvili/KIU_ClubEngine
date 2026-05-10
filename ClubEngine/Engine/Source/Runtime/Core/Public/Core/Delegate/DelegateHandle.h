#pragma once
#include <Core/Types/CoreTypes.h>

namespace CE
{
    struct DelegateHandle
    {
        uint64 value = 0;

        bool IsValid()                         const;
        bool operator==(const DelegateHandle&) const;
        bool operator!=(const DelegateHandle&) const;

        static DelegateHandle Generate();
        static DelegateHandle Invalid();
        void Invalidate();
    };
}