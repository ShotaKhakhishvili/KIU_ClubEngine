#pragma once
#include <vector>
#include "Actor.h"

namespace World
{
    extern std::vector<Actor*> actors;

    template<typename T, typename... Args>
    T* CreateActor(Args&&... args)
    {
        static_assert(std::is_base_of<Actor, T>::value, "T must derive from Actor");

        T* actor = new T(std::forward<Args>(args)...);
        actor->OnActorBegin();
        actors.push_back(actor);
        return actor;
    }

    void DestroyActor(Actor* actor);
    void Update();
    void Draw();
}
