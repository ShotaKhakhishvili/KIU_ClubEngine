#pragma once
#include "Actor.h"
#include "AnimObject.h"
#include "Object.h"

enum class PlayerInteraction {
    SABook = 0,
    Barrier = 1
};

enum class PlayerState {
    Normal = 0,
    Crouching = 1,
    Jumping = 2,
    GameOver = 3
};

class Player : public AnimObject
{
    PlayerState state;

    public:
    Player();

    void Update(double dTime) override;
    void PlayerInteracted(PlayerInteraction playerInteraction);
};