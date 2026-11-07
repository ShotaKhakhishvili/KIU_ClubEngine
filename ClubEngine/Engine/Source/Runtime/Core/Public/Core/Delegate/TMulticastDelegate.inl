#pragma once
#include <algorithm>
#include <cassert>
#include <cstring>

namespace CE
{
    template<typename... Args>
    FDelegateHandle TMulticastDelegate<Args...>::Add(std::function<void(Args...)> fn)
    {
        Entry e;

        e.handle = FDelegateHandle::Generate();
        e.fn = std::move(fn);

        e.instance  = nullptr;

        const FDelegateHandle handle = e.handle;

        AddEntry(std::move(e));

        return handle;
    }

    template<typename... Args>
    template<typename T>
    FDelegateHandle TMulticastDelegate<Args...>::Add(T* instance, void(T::*fn)(Args...))
    {
        static_assert(sizeof(fn) <= sizeof(Entry::fnBytes), "Member function pointer too large");

        Entry e;

        e.handle = FDelegateHandle::Generate();
        e.fn = [instance, fn](Args... args){ (instance->*fn)(args...);};
        e.instance = static_cast<void*>(instance);
        std::memcpy(e.fnBytes, &fn, sizeof(fn));

        const FDelegateHandle handle = e.handle;

        AddEntry(std::move(e));

        return handle;
    }
    template <typename... Args>
    template <typename T>
    FDelegateHandle TMulticastDelegate<Args...>::Add(T* instance, void (T::*fn)(Args...) const)
    {
        static_assert(sizeof(fn) <= sizeof(Entry::fnBytes), "Member function pointer too large");

        Entry e;

        e.handle = FDelegateHandle::Generate();
        e.fn = [instance, fn](Args... args){ (instance->*fn)(args...);};
        e.instance = static_cast<const void*>(instance);
        std::memcpy(e.fnBytes, &fn, sizeof(fn));

        const FDelegateHandle handle = e.handle;

        AddEntry(std::move(e));

        return handle;
    }

    template <typename... Args>
    void TMulticastDelegate<Args...>::Remove(FDelegateHandle handle)
    {
        if(!handle.IsValid())
            return;

        RemoveFirst([&](const Entry& e){
            return e.handle == handle;
        });
    }

    template<typename... Args>
    template<typename T>
    void TMulticastDelegate<Args...>::Remove(T* instance, void(T::*fn)(Args...))
    {
        unsigned char key[sizeof(Entry::fnBytes)]{};
        std::memcpy(&key, &fn, sizeof(fn));

        RemoveFirst([&](const Entry& e){
            return e.instance == static_cast<const void*>(instance) &&
                    std::memcmp(e.fnBytes, key, sizeof(key)) == 0;
        });
    }

    template <typename... Args>
    template <typename T>
    void TMulticastDelegate<Args...>::Remove(T* instance, void (T::*fn)(Args...) const)
    {
        unsigned char key[sizeof(Entry::fnBytes)]{};
        std::memcpy(&key, &fn, sizeof(fn));

        RemoveFirst([key, instance](const Entry& e){
            return e.instance == static_cast<const void*>(instance) &&
                    std::memcmp(e.fnBytes, key, sizeof(key)) == 0;
        });
    }

    template <typename... Args>
    void TMulticastDelegate<Args...>::Broadcast(Args... args) const
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
    void TMulticastDelegate<Args...>::Clear()
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
    bool TMulticastDelegate<Args...>::IsBound() const
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
    void TMulticastDelegate<Args...>::AddEntry(Entry&& entry)
    {
        if(broadcastDepth > 0)
            pendingAdds.push_back(std::move(entry));
        else
            handlers.push_back(std::move(entry));
    }

    template<typename... Args>
    void TMulticastDelegate<Args...>::Flush() const
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

template<typename... Args>
template<typename Pred>
void TMulticastDelegate<Args...>::RemoveFirst(Pred pred)
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

}