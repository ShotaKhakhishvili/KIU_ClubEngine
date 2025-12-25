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
    if (state != PlayerState::GameOver || 1) {
        AnimObject::Update(dTime);
        SetPosition(GetPosition() + glm::vec3(dTime * 4, 0, 0));
    }
    Camera::SetPosition(GetPosition() + glm::vec3(-4, 3, 0));
    Camera::SetRotation(GetRotation() + glm::vec3(-20, -90, 0));
    //Camera::SetPosition(GetPosition() + glm::vec3(-80, 60, 0));
    //Camera::SetRotation(GetRotation() + glm::vec3(-30, -90, 0));
}

Player::~Player() {
    if (scoreText) {
        World::DestroyActor(scoreText);  // if World supports this
        scoreText = nullptr;
    }
}


void Player::RefreshText() {
    scoreText->SetText("Score:" + std::to_string(score));
}