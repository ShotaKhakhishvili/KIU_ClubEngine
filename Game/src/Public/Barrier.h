#pragma once
#include "PlayerDetector.h"

class Barrier : public PlayerDetector {
public:
    Barrier(Player* player, glm::vec3 pos);
};
