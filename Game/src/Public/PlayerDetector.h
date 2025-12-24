#pragma once
#include "Object.h"
#include "Player.h"

class PlayerDetector : public Object{
    Player* player;
public:
    PlayerDetector(const char* model, const char* texture, const char* fragShader, Player* player);
    void Update(double dTime) override;
};
