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

        void Remove(FDelegateHandle handle);

        template<typename T>
        void Remove(T* instance, void(T::*fn)(Args...));

        void Broadcast(Args... args) const;
        void Clear();
        bool IsBound() const;

    private:
        struct Entry
        {
            FDelegateHandle                  handle                              ;
            std::function<void(Args...)>    fn                                  ;
            void*                           instance                = nullptr   ;
            alignas(void*) unsigned char    fnBytes[32]{}                       ;
            bool                            removed                 = false     ;
        };

        void AddEntry(Entry&& entry);
        template<typename Pred> void RemoveFirst(Pred pred)
        {
            auto it = std::find_if(handlers.begin(), handlers.end(), 
                [&](const Entry& e)
                { 
                    return !e.removed && pred(e);
                }
            );

            if(it != handlers.end())
            {
                if(broadcastDepth > 0)
                {
                    it->removed = true;
                    pendingRemoves = true;
                }
                else
                    handlers.erase(it);
                
                return;
            }

            // This happens if the handler was added during the broadcasting, and it is not yet added
            // to the handlers vector, so we have to remove it from the pendingAdds vector, which is not
            // accessed during broadcasting, so we can freely remove an element from it.
            auto pendingIt = std::find_if(pendingAdds.begin(), pendingAdds.end(), pred);
            if(pendingIt != pendingAdds.end())
                pendingAdds.erase(pendingIt);
        }
        void Flush()    const;

        mutable std::vector<Entry>          handlers;
        mutable std::vector<Entry>          pendingAdds;
        
        mutable uint32                      broadcastDepth = 0;
        mutable bool                        pendingRemoves = false;

    };
}

#include <Core/Delegate/TMulticastDelegate.inl>
