#pragma once

#include "BarrierShuttle.h"
#include "World.h"

#include <unordered_map>
#include <utility>

class BarrierShuttleLit : public BarrierShuttle
{
private:
    // instanceID -> (leftLightID, rightLightID)
    std::unordered_map<
        unsigned int,
        std::pair<World::LightID, World::LightID>
    > lightIndices;

public:
    explicit BarrierShuttleLit(Player* player);
    ~BarrierShuttleLit() override;

    unsigned int AddInstance(glm::vec3 position) override;
    void RemoveInstance(unsigned int id) override;
};
