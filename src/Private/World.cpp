#include "World.h"
#include <algorithm>

namespace World
{
    std::vector<Actor*> actors;
    std::vector<Light> lights;
    std::vector<Actor*> toDelete;

    double worldTime = 0;
    constexpr int MAX_LIGHTS = 32;

    void UploadLights(Shader& shader)
    {
        shader.Activate();

        int count = static_cast<int>(World::lights.size());
        if (count > MAX_LIGHTS)
            count = MAX_LIGHTS;

        glUniform1i(
            glGetUniformLocation(shader.getID(), "lightCount"),
            count
        );

        for (int i = 0; i < count; i++)
        {
            const Light& l = World::lights[i];

            std::string base = "lights[" + std::to_string(i) + "]";

            glUniform3fv(
                glGetUniformLocation(shader.getID(), (base + ".position").c_str()),
                1, &l.position[0]
            );

            glUniform3fv(
                glGetUniformLocation(shader.getID(), (base + ".color").c_str()),
                1, &l.color[0]
            );

            glUniform1f(
                glGetUniformLocation(shader.getID(), (base + ".intensity").c_str()),
                l.intensity
            );

            glUniform1f(
                glGetUniformLocation(shader.getID(), (base + ".radius").c_str()),
                l.radius
            );
        }
    }

    void DestroyActor(Actor* actor)
    {
        if (!actor) return;
        if (actor->pendingKill) return;

        actor->pendingKill = true;
        actor->OnActorEnd();
        toDelete.push_back(actor);
    }

    void FlushDestroyActors()
    {
        for (Actor* actor : toDelete)
        {
            auto it = std::find(actors.begin(), actors.end(), actor);
            if (it != actors.end())
                actors.erase(it);

            delete actor;
        }

        toDelete.clear();
    }

    void Update()
    {
        double currentTime = glfwGetTime();

        const size_t count = actors.size();
        for (size_t i = 0; i < count; i++)
        {
            if (!actors[i]->pendingKill)
                actors[i]->Update(currentTime - worldTime);
        }

        worldTime  = currentTime;

        FlushDestroyActors();
    }

    void Draw()
    {
        const size_t count = actors.size();
        for (size_t i = 0; i < count; i++)
        {
            Actor* actor = actors[i];
            if (actor->pendingKill)
                continue;
            if (!actor->IsWidget)
            {
                Shader* shader = actor->GetShader();
                if (shader)
                    UploadLights(*shader);
            }
            actor->Draw();
        }
    }

    unsigned int AddLight(glm::vec3 position, glm::vec3 color, float intensity, float radius)
    {
        lights.push_back(Light(position, color, intensity, radius));
        return lights.size() - 1;
    }
}
