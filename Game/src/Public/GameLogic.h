#pragma once

#include <vector>
#include <random>
#include <stdexcept>

#include "Actor.h"

class Player;
class Sublevel;
class SABook;
class Barrier;
struct Placeholderinfo;

class GameLogic : public Actor
{
    int lastSubLevelIndex = -1;
    void MakeNewSubLevel(unsigned int levelIdx);
    const std::vector<Placeholderinfo>& GetRandomSubLevel();

    Player* player = nullptr;

    std::vector<Sublevel*> subLevels;

    double furthestSubLevel      = 30.0;
    double cullDistance          = -100.0;
    double forwardCullDistance   = 100.0;
    double distBetweenLevels     = 1.0;

    double spawned               = 0.0;
    double lastTime              = 0.0;

public:
    static SABook*  book;
    static Barrier* barrierA;
    static Barrier* barrierBoard;
    static Barrier* shuttle;
    static Barrier* shuttleLit;
    static Barrier* board;

    GameLogic(Player* player);
    ~GameLogic() override;

    void Update(double dTime) override;

    void OnRestart();
};
