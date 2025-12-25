#pragma once
#include "InstancedObject.h"
#include "Object.h"
#include "Player.h"

class PlayerDetector : public InstancedObject{

    PlayerInteraction interactionType;

public:
    Player* player;

    PlayerDetector(const char* model, const char* texture, const char* fragShader, Player* player, PlayerInteraction interactionType);
    void Update(double dTime)override;
    void OnPlayerInteraction(unsigned int idx);
    virtual bool DetectPlayer(glm::vec3 instPosition);
};
