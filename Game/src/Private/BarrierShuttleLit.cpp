#include "BarrierShuttleLit.h"

#include "World.h"

BarrierShuttleLit::BarrierShuttleLit(Player* player)
    : BarrierShuttle(player)
{
}

BarrierShuttleLit::~BarrierShuttleLit()
{
    // Clean up any remaining lights we still own
    for (auto& [instanceID, lights] : lightIndices)
    {
        World::RemoveLight(lights.first);
        World::RemoveLight(lights.second);
    }

    lightIndices.clear();
}

unsigned int BarrierShuttleLit::AddInstance(glm::vec3 position)
{
    // Create the base shuttle instance
    unsigned int instanceID = BarrierShuttle::AddInstance(position);

    // Light parameters
    const glm::vec3 color     = { 1.0f, 0.0f, 0.0f };
    const float     intensity = 1.0f;
    const float     radius    = 3.0f;

    // Create lights
    World::LightID leftLight = World::AddLight(
        position + glm::vec3(-2.5f, 1.0f,  0.5f),
        color,
        intensity,
        radius
    );

    World::LightID rightLight = World::AddLight(
        position + glm::vec3(-2.5f, 1.0f, -0.5f),
        color,
        intensity,
        radius
    );

    // Store ownership
    lightIndices.emplace(
        instanceID,
        std::make_pair(leftLight, rightLight)
    );

    return instanceID;
}

void BarrierShuttleLit::RemoveInstance(unsigned int id)
{
    // Remove owned lights first
    auto it = lightIndices.find(id);
    if (it != lightIndices.end())
    {
        World::RemoveLight(it->second.first);
        World::RemoveLight(it->second.second);
        lightIndices.erase(it);
    }

    // Remove base instance
    BarrierShuttle::RemoveInstance(id);
}
