#pragma once
#include <Core/Delegate/DelegateHandle.h>
#include <functional>
#include <vector>
#include <cstring>

namespace CE
{
    template<typename... Args>
    class MulticastDelegate
    {
    public:
        DelegateHandle Add(std::function<void(Args...)> fn);

        template<typename T>
        DelegateHandle Add(T* instance, void(T::*fn)(Args...));

        void Remove(DelegateHandle handle);

        template<typename T>
        void Remove(T* instance, void(T::*fn)(Args...));

        void Broadcast(Args... args) const;
        void Clear();
        bool IsBound() const;

    private:
        struct Entry
        {
            DelegateHandle               handle;
            std::function<void(Args...)> fn;
            void*                        instance = nullptr;
            void*                        fnPtr    = nullptr;
        };

        mutable std::vector<Entry>          handlers;
        mutable bool                        broadcasting = false;
        mutable std::vector<DelegateHandle> pendingRemoves;

        void FlushRemove(DelegateHandle handle);
        void FlushPendingRemoves() const;
    };
}

#include <Core/Delegate/MulticastDelegate.inl>