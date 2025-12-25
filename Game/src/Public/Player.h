#pragma once
#include "Actor.h"
#include "AnimObject.h"
#include "Object.h"
#include "TextRenderer.h"

class PlayerDetector;

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
    PlayerState state = PlayerState::Normal;
    TextRenderer* scoreText = nullptr;
    GLFWwindow* window = nullptr;

    float moveSpeed = 10.0f;

    unsigned int score = 0;
    double zGoal = 0;


    bool noUpLastTime = true;
    bool noLeftLastTime = true;
    bool noRightLastTime = true;
    bool noDownLastTime = true;

    public:
    Player(GLFWwindow* window);
    ~Player();

    void Update(double dTime) override;
    void PlayerInteracted(PlayerInteraction playerInteraction);
    void RefreshText();
    void HandleInput(double dTime);
    double DInterpTo(double current, double goal, double interpSpeed, double dTime);
};