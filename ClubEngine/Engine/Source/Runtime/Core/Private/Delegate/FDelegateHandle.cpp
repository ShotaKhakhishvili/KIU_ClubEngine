#include <Core/Delegate/FDelegateHandle.h>
#include <atomic>

namespace CE
{
    bool FDelegateHandle::IsValid()                              const { return value != 0;          }
    bool FDelegateHandle::operator==(const FDelegateHandle& o)    const { return value == o.value;    }
    bool FDelegateHandle::operator!=(const FDelegateHandle& o)    const { return value != o.value;    }

    FDelegateHandle FDelegateHandle::Invalid() { return FDelegateHandle{0}; }
    void FDelegateHandle::Invalidate() { this->value = 0; }

    FDelegateHandle FDelegateHandle::Generate()
    {
        static std::atomic<uint64> counter{1};
        return FDelegateHandle{ counter.fetch_add(1, std::memory_order_relaxed) };
    }
}