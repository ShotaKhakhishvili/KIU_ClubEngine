#include "PlayerDetector.h"

PlayerDetector::PlayerDetector(const char *model, const char *texture, const char *fragShader, Player *player, PlayerInteraction interactionType)
    : Object(model, texture, fragShader)
{
    this->player = player;
}

void PlayerDetector::Update(double dTime) {
    Object::Update(dTime);

    float dist = glm::length(glm::vec3(player->GetPosition() - GetPosition()));
    if (dist < 0.5)
        player->PlayerInteracted(interactionType);
}