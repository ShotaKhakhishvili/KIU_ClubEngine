#include "World.h"
#include <algorithm>

namespace World
{
    std::vector<Actor*> actors;

    void DestroyActor(Actor* actor)
    {
        if (!actor) return;

        actor->OnActorEnd();

        auto it = std::find(actors.begin(), actors.end(), actor);
        if (it != actors.end())
            actors.erase(it);

        delete actor;
    }

    void Update()
    {
        for (unsigned int i = 0; i < actors.size(); i++)
        {
            actors[i]->Update();
        }
    }

    void Draw()
    {
        for (unsigned int i = 0; i < actors.size(); i++)
        {
            actors[i]->Draw();
        }
    }
}
