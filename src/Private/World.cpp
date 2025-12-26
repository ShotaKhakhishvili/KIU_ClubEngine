#include "World.h"
#include <algorithm>

namespace World
{
    std::vector<Actor*> actors;
    std::vector<Actor*> toDelete;

    std::unordered_map<LightID, Light> lights;

    double worldTime = 0.0;

    constexpr int MAX_LIGHTS = 32;
    static LightID nextLightID = 1; // 0 reserved as "invalid"

    // ---------------- LIGHT MANAGEMENT ----------------

    LightID AddLight(
        const glm::vec3& position,
        const glm::vec3& color,
        float intensity,
        float radius
    )
    {
        LightID id = nextLightID++;
        lights.emplace(id, Light{ position, color, intensity, radius });
        return id;
    }

    void RemoveLight(LightID id)
    {
        lights.erase(id);
    }

    bool HasLight(LightID id)
    {
        return lights.find(id) != lights.end();
    }

    Light* GetLight(LightID id)
    {
        auto it = lights.find(id);
        return (it != lights.end()) ? &it->second : nullptr;
    }

    // ---------------- SHADER UPLOAD ----------------

    void UploadLights(Shader& shader)
    {
        shader.Activate();

        int count = 0;
        glUniform1i(
            glGetUniformLocation(shader.getID(), "lightCount"),
            std::min((int)lights.size(), MAX_LIGHTS)
        );

        for (const auto& [id, l] : lights)
        {
            if (count >= MAX_LIGHTS)
                break;

            std::string base = "lights[" + std::to_string(count) + "]";

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

            count++;
        }
    }

    // ---------------- ACTOR LIFETIME ----------------

    void DestroyActor(Actor* actor)
    {
        if (!actor || actor->pendingKill)
            return;

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
        double delta = currentTime - worldTime;

        for (Actor* actor : actors)
        {
            if (!actor->pendingKill)
                actor->Update(delta);
        }

        worldTime = currentTime;
        FlushDestroyActors();
    }

    void Draw()
    {
        for (Actor* actor : actors)
        {
            if (!actor || actor->pendingKill)
                continue;

            if (actor->IsWidget)
                continue;

            Shader* shader = actor->GetShader();
            if (shader)
                UploadLights(*shader);

            actor->Draw();
        }

        GLboolean prevDepthTest = glIsEnabled(GL_DEPTH_TEST);
        GLint prevDepthFunc = GL_LESS;
        glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFunc);

        GLboolean prevBlend = glIsEnabled(GL_BLEND);
        GLint prevBlendSrcRGB = GL_ONE, prevBlendDstRGB = GL_ZERO;
        GLint prevBlendSrcA   = GL_ONE, prevBlendDstA   = GL_ZERO;
        glGetIntegerv(GL_BLEND_SRC_RGB, &prevBlendSrcRGB);
        glGetIntegerv(GL_BLEND_DST_RGB, &prevBlendDstRGB);
        glGetIntegerv(GL_BLEND_SRC_ALPHA, &prevBlendSrcA);
        glGetIntegerv(GL_BLEND_DST_ALPHA, &prevBlendDstA);

        GLboolean prevDepthMask = GL_TRUE;
        glGetBooleanv(GL_DEPTH_WRITEMASK, &prevDepthMask);

        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (Actor* actor : actors)
        {
            if (!actor || actor->pendingKill)
                continue;

            if (!actor->IsWidget)
                continue;

            actor->Draw();
        }

        glDepthMask(prevDepthMask);

        if (prevDepthTest) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);

        glDepthFunc(prevDepthFunc);

        if (prevBlend) glEnable(GL_BLEND);
        else glDisable(GL_BLEND);

        glBlendFuncSeparate(prevBlendSrcRGB, prevBlendDstRGB, prevBlendSrcA, prevBlendDstA);
    }

}
