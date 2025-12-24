#include "PlayerDetector.h"

void PlayerDetector::Update(double dTime) {
    Object:Update(dTime);

    float dist = glm::length(GetPosition() - GetPosition());

    std::cout << "distance: "<< dist << std::endl;
}

PlayerDetector::PlayerDetector(const char *model, const char *texture, const char *fragShader, Player *player) : Object(model, texture, fragShader) {
    this->player = player;
}