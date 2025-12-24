#include "PlayerDetector.h"

void PlayerDetector::Update(double dTime) {
    Object::Update(dTime);
}

PlayerDetector::PlayerDetector(const char *model, const char *texture, const char *fragShader, Player *player)
    : Object(model, texture, fragShader)
{
    this->player = player;
}