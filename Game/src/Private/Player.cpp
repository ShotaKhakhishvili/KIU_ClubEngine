#include "Player.h"
#include "World.h"

Player::Player()
    : AnimObject("run_frame0",16,"WolfTexture.png", "default.frag")
{
    SetPosition({-5,0,0});
    SetScale({0.1,0.1,0.1});
    SetRotation({0,90,0});

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
    if (state != PlayerState::GameOver) {
        AnimObject::Update(dTime);
        SetPosition(GetPosition() + glm::vec3(dTime * 4, 0, 0));
    }
    Camera::SetPosition(GetPosition() + glm::vec3(-4, 3, 0));
    Camera::SetRotation(GetRotation() + glm::vec3(-20, -90, 0));
}

Player::~Player() {
    delete scoreText;
}

void Player::RefreshText() {
    scoreText->SetText("Score:" + std::to_string(score));
}