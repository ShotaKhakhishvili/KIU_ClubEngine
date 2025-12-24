#pragma once
#include "Object.h"
#include "PlayerDetector.h"

class SABook : public PlayerDetector{
    float rotationSpeed = 180;
    float movementSpeed = 5;

    double moveProgress = 0;

    public:
        SABook(Player* player, glm::vec3 pos);
        void Update(double dTime) override;
};
