#pragma once
#include "PlayerDetector.h"

class Barrier : public PlayerDetector {
public:
    Barrier(const char* modelPath, const char* texturePath, Player* player, PlayerInteraction interaction);
    unsigned int AddInstance(glm::vec3 position) override;
    bool DetectPlayer(glm::vec3 instPosition) override;

    virtual float GetColX() const { return 2.5f; }
    virtual float GetColY() const { return 1.5f; }
    virtual float GetColZ() const { return 0.75f; }

    virtual float GetColCoordX() const { return 0.0f; }
    virtual float GetColCoordY() const { return 0.0f; }
    virtual float GetColCoordZ() const { return 0.0f; }
};
