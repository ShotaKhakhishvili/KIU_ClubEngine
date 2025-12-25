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
        if (DetectPlayer(inst.position))
            toRemove.push_back(id);
    }

    for (unsigned int id : toRemove)
        OnPlayerInteraction(id);
}


void PlayerDetector::OnPlayerInteraction(unsigned int idx)
{
    if (!player || player->state == PlayerState::GameOver)
        return;

    player->PlayerInteracted(interactionType);

    if (interactionType == PlayerInteraction::SABook)
        RemoveInstance(idx);
}


bool PlayerDetector::DetectPlayer(glm::vec3 instPosition) {
    float dist = glm::length(player->GetPosition() + glm::vec3(0, 0.5, 0) - instPosition);
    return dist < 0.8f;
}