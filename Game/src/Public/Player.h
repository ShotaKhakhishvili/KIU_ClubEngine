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
    Rolling = 1,
    Jumping = 2,
    GameOver = 3
};

class Player : public AnimObject
{
    TextRenderer* scoreText = nullptr;
    GLFWwindow* window = nullptr;

    float moveSpeed = 10.0f;

    unsigned int score = 0;
    double zGoal = 0;


    bool noUpLastTime = true;
    bool noLeftLastTime = true;
    bool noRightLastTime = true;
    bool noDownLastTime = true;

    float jumpTime = 0.0f;
    float jumpBaseY = 0.0f;

    float jumpDuration = 0.65f;
    float jumpHeight = 1.0f;

    public:

    float colX = 0.275, colY = 1.5, colZ = 0.5;
    float colCoordX = 0.0f, colCoordY = 0.0f, colCoordZ = 0.0f;

    PlayerState state = PlayerState::Normal;
    bool alive = true;

    Player(GLFWwindow* window);
    ~Player();

    void Update(double dTime) override;
    void PlayerInteracted(PlayerInteraction playerInteraction);
    void RefreshText();
    void HandleInput(double dTime);

    double DInterpTo(double current, double goal, double interpSpeed, double dTime);
    glm::vec3 VInterpTo(glm::vec3 current, glm::vec3 goal, double interpSpeed, double dTime);

    void RefreshState();
    void HandleState();

    void SetCollisionToRoll();
    void SetCollisionToNormal();
    void UpdateJump(double dTime);
};