#pragma once
#include "Actor.h"
#include "Player.h"
#include "Sublevel.h"

class GameLogic : public Actor{

    Player* player;
    std::vector<Sublevel*> subLevels;
    double furthestSubLevel = 0.0f;
    double cullDistance = -5.0f;
    double forwardCullDistance = 10.0f;

    public:
        explicit GameLogic(Player* player);

        void Update(double deltaTime) override;
        static const std::vector<Placeholderinfo>& GetRandomSubLevel();
};