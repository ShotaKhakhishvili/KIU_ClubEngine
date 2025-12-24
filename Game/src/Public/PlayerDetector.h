#pragma once
#include "Object.h"
#include "Player.h"

class PlayerDetector : public Object{

    PlayerInteraction interactionType;
    Player* player;

public:
    PlayerDetector(const char* model, const char* texture, const char* fragShader, Player* player, PlayerInteraction interactionType);
    void Update(double dTime)override;
    void OnPlayerInteraction();
};
