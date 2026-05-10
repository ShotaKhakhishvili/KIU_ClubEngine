#include <Core/Delegate/DelegateHandle.h>
#include <atomic>

namespace CE
{
    bool DelegateHandle::IsValid()                              const { return value != 0;          }
    bool DelegateHandle::operator==(const DelegateHandle& o)    const { return value == o.value;    }
    bool DelegateHandle::operator!=(const DelegateHandle& o)    const { return value != o.value;    }

    DelegateHandle DelegateHandle::Invalid() { return DelegateHandle{0}; }
    void DelegateHandle::Invalidate() { this->value = 0; }

    DelegateHandle DelegateHandle::Generate()
    {
        static std::atomic<uint64> counter{1};
        return DelegateHandle{ counter.fetch_add(1, std::memory_order_relaxed) };
    }
}