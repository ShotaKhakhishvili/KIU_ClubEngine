#pragma once
#include <algorithm>

namespace CE
{
    template<typename... Args>
    DelegateHandle MulticastDelegate<Args...>::Add(std::function<void(Args...)> fn)
    {
        Entry e;

        e.handle = DelegateHandle::Generate();
        e.fn = std::move(fn);

        e.instance  = nullptr;
        e.fnPtr     = nullptr;

        handlers.push_back(std::move(e));

        return handlers.back().handle;
    }

    template<typename... Args>
    template<typename T>
    DelegateHandle MulticastDelegate<Args...>::Add(T* instance, void(T::*fn)(Args...))
    {
        Entry e;

        e.handle = DelegateHandle::Generate();
        e.fn = [instance, fn](Args... args){ (instance->*fn)(args...);};

        e.instance = static_cast<void*>(instance);
        std::memcpy(&e.fnPtr, &fn, sizeof(void*));

        handlers.push_back(std::move(e));
        return handlers.back().handle;
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::Remove(DelegateHandle handle)
    {
        if(!handle.IsValid())
            return;

        if(broadcasting)
        {
            pendingRemoves.push_back(handle);
            return;
        }

        FlushRemove(handle);
    }

    template<typename... Args>
    template<typename T>
    void MulticastDelegate<Args...>::Remove(T* instance, void(T::*fn)(Args...))
    {
        void* fnPtr = nullptr;
        std::memcpy(&fnPtr, &fn, sizeof(void*));

        auto it = std::find(handlers.begin(), handlers.end(),
            [instance, fnPtr](const Entry e){
                return e.instance == static_cast<void*>(instance) &&
                        fnPtr == e.fnPtr;
        });

        if(it == handlers.end()) 
            return;
        
        if(broadcasting)
            pendingRemoves.push_back(*it.handle);
        else
            handlers.erase(it);
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::Broadcast(Args... args) const
    {
        broadcasting = true;

        for(const Entry& entry : handlers)
        {
            if(entry.handle.IsValid())
                entry.fn(args...);
        }

        broadcasting = false;
        FlushPendingRemoves();
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::Clear()
    {
        if(broadcasting)
        {
            for(const Entry& entry : handlers)
                pendingRemoves.push_back(entry.handle);
            return;
        }

        handlers.clear();
    }

    template<typename... Args>
    bool MulticastDelegate<Args...>::IsBound() const
    {
        return !handlers.empty();
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::FlushRemove(DelegateHandle handle)
    {
        auto it = std::find_if(handlers.begin(), handlers.end(),
            [handle](const Entry& e){ 
                return e.handle == handle; 
        });

        if (it != handlers.end())
            handlers.erase(it);
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::FlushPendingRemoves() const
    {
        for(DelegateHandle& handle : pendingRemoves)
            const_cast<MulticastDelegate*>(this)->FlushRemove(handle);
     
        pendingRemoves.clear(); 
    }
}