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

    float dist = std::abs(player->GetPosition().x - GetPosition().x);
    if (dist < 0.25 && player->GetPosition().z == GetPosition().z)
        OnPlayerInteraction();
}

void PlayerDetector::OnPlayerInteraction() {
    player->PlayerInteracted(interactionType);

    if (interactionType == PlayerInteraction::SABook)
        World::DestroyActor(this);
}