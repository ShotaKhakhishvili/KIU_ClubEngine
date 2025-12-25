#pragma once
#include <vector>
#include <unordered_map>
#include <cstdint>
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
    using LightID = uint32_t;

    extern std::vector<Actor*> actors;
    extern double worldTime;

    // Lights
    extern std::unordered_map<LightID, Light> lights;

    template<typename T, typename... Args>
    T* CreateActor(Args&&... args)
    {
        static_assert(std::is_base_of<Actor, T>::value, "T must derive from Actor");

        T* actor = new T(std::forward<Args>(args)...);
        actor->OnActorBegin();
        actors.push_back(actor);
        return actor;
    }

    // Lights API
    LightID AddLight(
        const glm::vec3& position,
        const glm::vec3& color,
        float intensity,
        float radius
    );

    void RemoveLight(LightID id);
    bool HasLight(LightID id);
    Light* GetLight(LightID id);

    void UploadLights(Shader& shader);

    void DestroyActor(Actor* actor);
    void FlushDestroyActors();
    void Update();
    void Draw();
}
