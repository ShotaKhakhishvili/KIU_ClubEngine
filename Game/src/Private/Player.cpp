#include "Player.h"
#include "World.h"
#include "GameLogic.h"

#include <algorithm>
#include "Audio.h"
#include "Path.h"

Player::Player(GLFWwindow* win)
    : AnimObject(
        { "run_frame0", "my_animation.000", "roll_frame0", "fall_frame.0" },
        { 16, 62, 28, 50 },
        "WolfTexture.png",
        "default.frag"
      )
{
    window = win;

    SetPosition({ -5, 0, 0 });
    SetScale({ 0.1f, 0.1f, 0.1f });
    SetRotation({ 0, 90, 0 });

    scoreText = World::CreateActor<TextRenderer>();
    scoreText->SetPosition({ 20, TextRenderer::GetScreenHeight() - 56 });
    RefreshText();
}

Player::~Player()
{
    if (scoreText)
        World::DestroyActor(scoreText);

    if (gameOverText)
        World::DestroyActor(gameOverText);

    if (resetText)
        World::DestroyActor(resetText);
}

void Player::SetGameLogic(GameLogic* logic)
{
    gameLogic = logic;
}

void Player::PlayerInteracted(PlayerInteraction interaction)
{
    if (interaction == PlayerInteraction::Barrier)
    {
        state = PlayerState::GameOver;
    }
    else if (interaction == PlayerInteraction::SABook)
    {
        Audio::PlayFromTime(Path::Sound("paper.mp3"),0.20f,1.0, bookPitch);
        bookPitch += 0.15f;
        sinceLastBook = 0.0f;
        score++;
        RefreshText();
    }
}

void Player::Update(double dTime)
{
    AnimObject::Update(dTime);

    sinceLastBook += dTime;
    if (sinceLastBook > 1.3f) {
        bookPitch = 1.0f;
    }

    if (restarting)
        return;

    if (state != PlayerState::GameOver)
    {
        SetPosition(GetPosition() + glm::vec3(dTime * moveSpeed, 0, 0));

        HandleInput(dTime);
        UpdateJump(dTime);

        SetPosition({
            GetPosition().x,
            GetPosition().y,
            DInterpTo(GetPosition().z, zGoal, 5.0, dTime)
        });

        RefreshState();
        HandleState();
    }
    else
    {
        HandleRestartInput();
    }

    if (state == PlayerState::GameOver)
    {
        if (alive)
        {
            alive = false;
            Audio::Play(Path::Sound("gameover.mp3"),0.30f);
            PlayAnimationOnce(3, 0, 0, 1000, 1.0f);
            DisplayGameOverTexts();
        }


            Camera::SetPosition(
            VInterpTo(Camera::GetPosition(),
                 GetPosition() + glm::vec3(0, 10, 0),
                 0.4, dTime));

            glm::vec3 targetDir =
                Camera::DirectionFromEuler(glm::vec3(-90, 0, 0));

            Camera::SetOrientation(glm::normalize(
                VInterpTo(Camera::GetOrientation(), targetDir, 1.0, dTime)));

    }
    else
    {
            Camera::SetPosition(
                VInterpTo(Camera::GetPosition(),
                          GetPosition() + glm::vec3(-4, 3, 0),
                          10.0, dTime));

            Camera::SetRotation(GetRotation() + glm::vec3(-20, -90, 0));

    }
}

void Player::HandleInput(double)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (state != PlayerState::Jumping && noUpLastTime)
        {
            state = PlayerState::Jumping;
            jumpTime = 0.0f;
            jumpBaseY = GetPosition().y;
            PlayAnimationOnce(1, 10, 12, 0, 2.0f);
        }
        noUpLastTime = false;
    }
    else noUpLastTime = true;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (noLeftLastTime)
            zGoal = std::max(-2.0, zGoal - 2);
        noLeftLastTime = false;
    }
    else noLeftLastTime = true;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (noRightLastTime)
            zGoal = std::min(2.0, zGoal + 2);
        noRightLastTime = false;
    }
    else noRightLastTime = true;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        if (noDownLastTime && GetAnimIndex() != 2)
            PlayAnimationOnce(2, 0, 0, 0, 1.5f);
        noDownLastTime = false;
    }
    else noDownLastTime = true;

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        if (!alive && noEnterLastTime)
            OnRestart();
        noEnterLastTime = false;
    }
    else noEnterLastTime = true;
}

void Player::OnRestart()
{
    restarting = true;
    SetPosition({ -5, 0, 0 });
    zGoal = 0.0;

    if (gameLogic)
        gameLogic->OnRestart();

    state = PlayerState::Normal;
    alive = true;

    score = 0;

    jumpTime = 0.0f;
    jumpBaseY = 0.0f;

    noUpLastTime = true;
    noLeftLastTime = true;
    noRightLastTime = true;
    noDownLastTime = true;
    noEnterLastTime = true;

    SetCollisionToNormal();

    SetRotation({ 0, 90, 0 });

    PlayAnimationLoop(0, 0, 0, 1.0f);

    Camera::SetPosition(GetPosition() + glm::vec3(-4, 3, 0));
    Camera::SetRotation(GetRotation() + glm::vec3(-20, -90, 0));

    scoreText->SetPosition({ 20, TextRenderer::GetScreenHeight() - 56 });
    RefreshText();

    if (gameOverText)
        World::DestroyActor(gameOverText), gameOverText = nullptr;

    if (resetText)
        World::DestroyActor(resetText), resetText = nullptr;

    restarting = false;
}

void Player::DisplayGameOverTexts()
{
    constexpr float GAME_OVER_SIZE = 2.5f;
    constexpr float RESTART_SIZE   = 1.2f;

    auto estimateWidth = [](const std::string& s, float size)
    {
        return s.length() * 24.0f * size;
    };

    float cx = TextRenderer::GetScreenWidth() * 0.5f;
    float cy = TextRenderer::GetScreenHeight() * 0.5f;

    gameOverText = World::CreateActor<TextRenderer>();
    gameOverText->SetText("Game Over");
    gameOverText->SetSize(GAME_OVER_SIZE);
    gameOverText->SetColor({ 1.0f, 0.2f, 0.2f });

    gameOverText->SetPosition({
        cx - estimateWidth("Game Over", GAME_OVER_SIZE) * 0.5f,
        cy + 30.0f
    });

    resetText = World::CreateActor<TextRenderer>();
    resetText->SetText("Press Enter To Restart");
    resetText->SetSize(RESTART_SIZE);
    resetText->SetColor({ 1.0f, 1.0f, 1.0f });

    resetText->SetPosition({
        cx - estimateWidth("Press Enter To Restart", RESTART_SIZE) * 0.5f,
        cy - 30.0f
    });
}

void Player::RefreshText()
{
    scoreText->SetText("GPA: 0." + std::to_string(score));
}

void Player::RefreshState()
{
    if (state == PlayerState::GameOver || state == PlayerState::Jumping)
        return;

    state = (GetAnimIndex() == 2)
        ? PlayerState::Rolling
        : PlayerState::Normal;
}

void Player::HandleState()
{
    if (state == PlayerState::Rolling)
        SetCollisionToRoll();
    else
        SetCollisionToNormal();
}

void Player::SetCollisionToNormal()
{
    colCoordX = colCoordY = colCoordZ = 0.0f;
    colY = 1.5f;
}

void Player::SetCollisionToRoll()
{
    colCoordX = colCoordY = colCoordZ = 0.0f;
    colY = 0.2f;
}

void Player::UpdateJump(double dTime)
{
    if (state != PlayerState::Jumping)
        return;

    if (jumpTime == 0)
        Audio::PlayFromTime(Path::Sound("jump.mp3"), 0.3f, 0.8f);

    jumpTime += static_cast<float>(dTime);
    float t = std::clamp(jumpTime / jumpDuration, 0.0f, 1.0f);

    float offset = std::sin(t * glm::pi<float>()) * jumpHeight;

    glm::vec3 pos = GetPosition();
    pos.y = jumpBaseY + offset;
    SetPosition(pos);

    if (t >= 1.0f)
    {
        pos.y = jumpBaseY;
        SetPosition(pos);
        state = PlayerState::Normal;
    }
}

double Player::DInterpTo(double current, double goal, double speed, double dt)
{
    if (speed <= 0.0)
        return goal;

    double delta = goal - current;
    if (std::abs(delta) < 1e-6)
        return goal;

    double step = std::clamp(speed * dt, 0.0, 1.0);
    return current + delta * step;
}

glm::vec3 Player::VInterpTo(glm::vec3 current, glm::vec3 goal, double speed, double dt)
{
    if (speed <= 0.0)
        return goal;

    glm::vec3 delta = goal - current;
    if (glm::length2(delta) < 1e-12)
        return goal;

    double step = std::clamp(speed * dt, 0.0, 1.0);
    return current + delta * static_cast<float>(step);
}

void Player::HandleRestartInput()
{
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        if (noEnterLastTime)
            OnRestart();

        noEnterLastTime = false;
    }
    else
    {
        noEnterLastTime = true;
    }
}
