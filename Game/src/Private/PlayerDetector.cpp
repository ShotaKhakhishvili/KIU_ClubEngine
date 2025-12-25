#include "PlayerDetector.h"

#include "World.h"

PlayerDetector::PlayerDetector(const char *model, const char *texture, const char *fragShader, Player *player, PlayerInteraction interactionType)
    : InstancedObject(model, texture, fragShader)
{
    this->interactionType = interactionType;
    this->player = player;
}

void PlayerDetector::Update(double dTime)
{
    InstancedObject::Update(dTime);

    std::vector<unsigned int> toRemove;

    for (auto& [id, inst] : objInsts)
    {
        float dist = glm::length(player->GetPosition() + glm::vec3(0, 0.5, 0) - inst.position);
        if (dist < 0.75f)
            toRemove.push_back(id);
    }

    for (unsigned int id : toRemove)
        OnPlayerInteraction(id);
}


void PlayerDetector::OnPlayerInteraction(unsigned int idx) {
    player->PlayerInteracted(interactionType);

    if (interactionType == PlayerInteraction::SABook)
        RemoveInstance(idx);
}