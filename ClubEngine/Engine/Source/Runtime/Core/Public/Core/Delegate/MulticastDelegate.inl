#pragma once
#include <algorithm>
#include <cassert>

namespace CE
{
    template<typename... Args>
    DelegateHandle MulticastDelegate<Args...>::Add(std::function<void(Args...)> fn)
    {
        Entry e;

        e.handle = DelegateHandle::Generate();
        e.fn = std::move(fn);

        e.instance  = nullptr;

        const DelegateHandle handle = e.handle;

        if(broadcastDepth > 0)
            pendingAdds.push_back(std::move(e));

        else
            handlers.push_back(std::move(e));

        return handle;
    }

    template<typename... Args>
    template<typename T>
    DelegateHandle MulticastDelegate<Args...>::Add(T* instance, void(T::*fn)(Args...))
    {
        static_assert(sizeof(&fn) <= sizeof(Entry::fnBytes), "Member function pointer too large");

        Entry e;

        e.handle = DelegateHandle::Generate();
        e.fn = [instance, fn](Args... args){ (instance->*fn)(args...);};
        e.instance = static_cast<void*>(instance);
        std::memcpy(e.fnBytes, &fn, sizeof(void*));

        const DelegateHandle handle = e.handle;

        if(broadcastDepth > 0)
            pendingAdds.push_back(std::move(e));

        else
            handlers.push_back(std::move(e));

        return handle;
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::Remove(DelegateHandle handle)
    {
        if(!handle.IsValid())
            return;

        auto it = std::find_if(handlers.begin(), handlers.end(),
            [&](const Entry& e){
                return e.handle == handle;
        });

        if(it == handlers.end())
            return;

        if(broadcastDepth > 0)
        {
            it->removed = true;
            pendingRemoves = true;
        }
        else
            handlers.erase(it);
    }

    template<typename... Args>
    template<typename T>
    void MulticastDelegate<Args...>::Remove(T* instance, void(T::*fn)(Args...))
    {
        unsigned char key[sizeof(Entry::fnBytes)]{};
        std::memcpy(&key, &fn, sizeof(void*));

        auto it = std::find_if(handlers.begin(), handlers.end(),
            [&](const Entry& e){
                return e.instance == static_cast<void*>(instance) &&
                        std::memcmp(e.fnBytes, key, sizeof(key)) == 0;
        });

        if(it == handlers.end()) 
            return;
        
        if(broadcastDepth > 0)
        {
            it->removed = true;
            pendingRemoves = true;
        }
        else
            handlers.erase(it);
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::Broadcast(Args... args) const
    {
        ++broadcastDepth;

        for(size_t i = 0; i < handlers.size(); i++)
        {
            if(!handlers[i].removed)
                handlers[i].fn(args...);
        }

        if(--broadcastDepth == 0)
            Flush();
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::Clear()
    {
        pendingAdds.clear();

        if(broadcastDepth > 0)
        {
            for(Entry& e : handlers) e.removed = true;
            pendingRemoves = !handlers.empty();
            return;
        }

        handlers.clear();
    }

    template<typename... Args>
    bool MulticastDelegate<Args...>::IsBound() const
    {
        return !pendingAdds.empty() || 
                std::any_of(handlers.begin(), handlers.end(), 
                    [](const Entry& e)
                    {
                        return !e.removed;
                    }
                );
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::AddEntry(Entry&& entry)
    {
        if(broadcastDepth > 0)
            pendingAdds.push_back(std::move(entry));
        else
            handlers.push_back(std::move(entry));
    }

    template<typename... Args>
    void MulticastDelegate<Args...>::Flush() const
    {
        if(pendingRemoves)
        {
            std::erase_if(handlers,
                [](const Entry& e)
                {
                    return e.removed;
                }
            );
            pendingRemoves = false;
        }

        for(Entry& e : pendingAdds)
            handlers.push_back(std::move(e));

        pendingAdds.clear();
    }
}