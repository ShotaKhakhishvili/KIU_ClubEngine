#pragma once
#include "Object.h"
#include "PlayerDetector.h"

class SABook : public PlayerDetector{
    float rotationSpeed = 180;
    float movementSpeed = 5;

    double moveProgress = 0;

    std::unordered_map<unsigned int, double> baseYs;

    public:
        SABook(Player* player);
        void Update(double dTime) override;
        unsigned int AddInstance(glm::vec3 position) override;
};
