#include "PlayerDetector.h"

#include "World.h"

PlayerDetector::PlayerDetector(const char *model, const char *texture, const char *fragShader, Player *player, PlayerInteraction interactionType)
    : Object(model, texture, fragShader)
{
    this->interactionType = interactionType;
    this->player = player;
}

void PlayerDetector::Update(double dTime) {
    Object::Update(dTime);

    float dist = glm::length(glm::vec3(player->GetPosition() - GetPosition()));
    if (dist < 0.5)
        OnPlayerInteraction();
}

void PlayerDetector::OnPlayerInteraction() {
    player->PlayerInteracted(interactionType);
    World::DestroyActor(this);
}