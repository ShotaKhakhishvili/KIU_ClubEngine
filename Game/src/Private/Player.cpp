#include "Player.h"
#include "World.h"
#include <algorithm>

extern int WINDOW_HEIGHT;

Player::Player(GLFWwindow* win)
    : AnimObject({"run_frame0", "my_animation.000", "roll_frame0", "fall_frame.0"},{16, 62, 28, 50},"WolfTexture.png", "default.frag")
{
    SetPosition({-5,0,0});
    SetScale({0.1,0.1,0.1});
    SetRotation({0,90,0});

    window = win;

    scoreText = World::CreateActor<TextRenderer>();
    scoreText->SetPosition({20, WINDOW_HEIGHT - 56});
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
    AnimObject::Update(dTime);
    if (state != PlayerState::GameOver) {
        SetPosition(GetPosition() + glm::vec3(dTime * moveSpeed, 0, 0));

        HandleInput(dTime);
        UpdateJump(dTime);

        SetPosition({GetPosition().x, GetPosition().y, DInterpTo(GetPosition().z, zGoal, 5.0, dTime)});

        RefreshState();
        HandleState();
    }
    else if (alive) {
        alive = false;
        PlayAnimationOnce(3, 0, 0, 1000, 1.0f);
    }

    if (state == PlayerState::GameOver) {
        Camera::SetPosition(VInterpTo(Camera::GetPosition(), GetPosition() + glm::vec3(0, 10, 0), 0.4, dTime));
        glm::vec3 targetDir = Camera::DirectionFromEuler(glm::vec3(-90,0,0));

        Camera::SetOrientation(
            glm::normalize(
                VInterpTo(Camera::GetOrientation(), targetDir, 1.0, dTime)
            )
        );
    }
    else {
        Camera::SetPosition(VInterpTo(Camera::GetPosition(), GetPosition() + glm::vec3(-4, 3, 0), 10, dTime));
        Camera::SetRotation(GetRotation() + glm::vec3(-20, -90, 0));
    }

    //Camera::SetPosition(GetPosition() + glm::vec3(-80, 60, 0));
    //Camera::SetRotation(GetRotation() + glm::vec3(-30, -90, 0));

}

Player::~Player() {
    if (scoreText) {
        World::DestroyActor(scoreText);
        scoreText = nullptr;
    }
}

void Player::RefreshText() {
    scoreText->SetText("GPA: 0." + std::to_string(score));
}

void Player::HandleInput(double dTime) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        if (state != PlayerState::Jumping && noUpLastTime){
            state = PlayerState::Jumping;
            jumpTime = 0.0f;
            jumpBaseY = GetPosition().y;

            PlayAnimationOnce(1, 10, 12, 0, 2.0f);
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

glm::vec3 Player::VInterpTo(
    glm::vec3 current,
    glm::vec3 goal,
    double interpSpeed,
    double dTime)
{
    if (interpSpeed <= 0.0)
        return goal;

    const glm::vec3 delta = goal - current;

    // Close enough → snap
    if (glm::length2(delta) < 1e-12)
        return goal;

    const double interpStep = std::clamp(interpSpeed * dTime, 0.0, 1.0);

    return current + delta * static_cast<float>(interpStep);
}


void Player::SetCollisionToNormal() {
    colCoordX = colCoordY = colCoordZ = 0;
    colY = 1.5;
}
void Player::SetCollisionToRoll() {
    colCoordX = colCoordY = colCoordZ = 0;
    colY = 0.2;
}

void Player::RefreshState()
{
    if (state == PlayerState::GameOver)
        return;
    if (state == PlayerState::Jumping)
        return;
    if (GetAnimIndex() == 2)
        state = PlayerState::Rolling;
    else
        state = PlayerState::Normal;
}


void Player::HandleState() {
    if (state == PlayerState::Normal) {
        SetCollisionToNormal();
    }
    else if (state == PlayerState::Rolling) {
        SetCollisionToRoll();
    }
}

void Player::UpdateJump(double dTime)
{
    if (state != PlayerState::Jumping)
        return;

    jumpTime += static_cast<float>(dTime);

    float t = jumpTime / jumpDuration;
    t = std::clamp(t, 0.0f, 1.0f);

    // Sine jump arc: 0 → peak → 0
    float jumpOffset = std::sin(t * glm::pi<float>()) * jumpHeight;

    glm::vec3 pos = GetPosition();
    pos.y = jumpBaseY + jumpOffset;
    SetPosition(pos);

    if (t >= 1.0f)
    {
        // Landing
        pos.y = jumpBaseY;
        SetPosition(pos);
        state = PlayerState::Normal;
    }
}


