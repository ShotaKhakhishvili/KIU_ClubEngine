#pragma once
#include "InstancedObject.h"
#include "Object.h"
#include "Player.h"

class PlayerDetector : public InstancedObject{

    PlayerInteraction interactionType;
    Player* player;

public:
    PlayerDetector(const char* model, const char* texture, const char* fragShader, Player* player, PlayerInteraction interactionType);
    void Update(double dTime)override;
    void OnPlayerInteraction(unsigned int idx);
};
