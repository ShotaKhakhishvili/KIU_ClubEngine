#pragma once
#include "Actor.h"
#include "Barrier.h"
#include "Player.h"
#include "Sublevel.h"
#include "SABook.h"


class GameLogic : public Actor{
    Player* player;
    std::vector<Sublevel*> subLevels;
    double furthestSubLevel = 30.0f;
    double cullDistance = -25.0f;
    double forwardCullDistance = 100.0f;
    double distBetweenLevels = 10.0f;

    double lastTime = 0.0f;
    double spawned = 0.0f;

    public:
        static SABook* book;
        static Barrier* barrierA;
        static Barrier* barrierBoard;
        static Barrier* shuttle;
        static Barrier* shuttleLit;
        static Barrier* board;

        explicit GameLogic(Player* player);
        ~GameLogic();

        void Update(double deltaTime) override;
        static const std::vector<Placeholderinfo>& GetRandomSubLevel();
    void MakeNewSubLevel(unsigned int levelIdx);
};