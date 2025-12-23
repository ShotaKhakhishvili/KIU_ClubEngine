#pragma once
#include <vector>
#include "Actor.h"

struct Light
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float radius;
};

namespace World
{
    extern std::vector<Actor*> actors;
    extern std::vector<Light> lights;

    template<typename T, typename... Args>
    T* CreateActor(Args&&... args)
    {
        static_assert(std::is_base_of<Actor, T>::value, "T must derive from Actor");

        T* actor = new T(std::forward<Args>(args)...);
        actor->OnActorBegin();
        actors.push_back(actor);
        return actor;
    }

    void UpdateLight(Shader& shader);

    void DestroyActor(Actor* actor);
    void Update();
    void Draw();

    unsigned int AddLight(glm::vec3 position, glm::vec3 color, float intensity, float radius);
}
