#include "Player.h"
#include "World.h"
#include <algorithm>

Player::Player(GLFWwindow* win)
    : AnimObject({"run_frame0", "my_animation.000", "roll_frame0"},{16, 62, 28},"WolfTexture.png", "default.frag")
{
    SetPosition({-5,0,0});
    SetScale({0.1,0.1,0.1});
    SetRotation({0,90,0});

    window = win;

    scoreText = World::CreateActor<TextRenderer>();
    RefreshText();
}

void Player::PlayerInteracted(PlayerInteraction playerInteraction) {
    if (playerInteraction == PlayerInteraction::Barrier) {
        state = PlayerState::GameOver;
    }
    else if (playerInteraction == PlayerInteraction::SABook) {
        score++;
        RefreshText();
    }
}

void Player::Update(double dTime)
{
    if (state != PlayerState::GameOver || 1) {
        AnimObject::Update(dTime);
        SetPosition(GetPosition() + glm::vec3(dTime * moveSpeed, 0, 0));
    }
    Camera::SetPosition(GetPosition() + glm::vec3(-4, 3, 0));
    Camera::SetRotation(GetRotation() + glm::vec3(-20, -90, 0));
    //Camera::SetPosition(GetPosition() + glm::vec3(-80, 60, 0));
    //Camera::SetRotation(GetRotation() + glm::vec3(-30, -90, 0));

    HandleInput(dTime);
    SetPosition({GetPosition().x, GetPosition().y, DInterpTo(GetPosition().z, zGoal, 5.0, dTime)});
}

Player::~Player() {
    if (scoreText) {
        World::DestroyActor(scoreText);
        scoreText = nullptr;
    }
}


void Player::RefreshText() {
    scoreText->SetText("Score:" + std::to_string(score));
}

void Player::HandleInput(double dTime) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (GetAnimIndex() != 1 && noUpLastTime) {
            PlayAnimationOnce(1, 10, 12, 0, 2.0f);
            std::cout << "space" << std::endl;
        }
        noUpLastTime = false;
    }
    else
        noUpLastTime = true;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (noLeftLastTime) {
            zGoal = std::max(-2.0, zGoal - 2);
        }
        noLeftLastTime = false;
    }
    else
        noLeftLastTime = true;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (noRightLastTime) {
            zGoal = std::min(2.0, zGoal + 2);
        }
        noRightLastTime = false;
    }
    else
        noRightLastTime = true;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (noDownLastTime) {
            PlayAnimationOnce(2, 0, 0, 0, 1.5f);
        }
        noDownLastTime = false;
    }
    else
        noDownLastTime = true;
}

double Player::DInterpTo(double current, double goal, double interpSpeed, double dTime)
{
    if (interpSpeed <= 0.0)
        return goal;

    const double delta = goal - current;

    if (std::abs(delta) < 1e-6)
        return goal;

    const double interpStep = std::clamp(interpSpeed * dTime, 0.0, 1.0);

    return current + delta * interpStep;
}
