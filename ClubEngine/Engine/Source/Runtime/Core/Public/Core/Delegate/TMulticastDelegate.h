#pragma once
#include <Core/Delegate/FDelegateHandle.h>
#include <functional>
#include <vector>

namespace CE
{
    template<typename... Args>
    class TMulticastDelegate
    {
    public:
        FDelegateHandle Add(std::function<void(Args...)> fn);

        template<typename T>
        FDelegateHandle Add(T* instance, void(T::*fn)(Args...));

        template<typename T>
        FDelegateHandle Add(T* instance, void(T::*fn)(Args...) const);

        void Remove(FDelegateHandle handle);

        template<typename T>
        void Remove(T* instance, void(T::*fn)(Args...));

        template<typename T>
        void Remove(T* instance, void(T::*fn)(Args...) const);

        void Broadcast(Args... args) const;
        void Clear();
        bool IsBound() const;

    private:
        struct Entry
        {
            FDelegateHandle                 handle                              ;
            std::function<void(Args...)>    fn                                  ;
            const void*                     instance                = nullptr   ;
            alignas(void*) unsigned char    fnBytes[32]{}                       ;
            bool                            removed                 = false     ;
        };

        void AddEntry(Entry&& entry);
        template<typename Pred>
        void RemoveFirst(Pred pred);

        void Flush()    const;

        mutable std::vector<Entry>          handlers;
        mutable std::vector<Entry>          pendingAdds;
        
        mutable uint32                      broadcastDepth = 0;
        mutable bool                        pendingRemoves = false;

    };
}

#include "TMulticastDelegate.inl"
