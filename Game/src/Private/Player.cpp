#include "Player.h"

Player::Player() :
AnimObject(
    {
        "run_frame01.txt",
        "run_frame02.txt",
        "run_frame03.txt",
        "run_frame04.txt",
        "run_frame05.txt",
        "run_frame06.txt",
        "run_frame07.txt",
        "run_frame08.txt",
        "run_frame09.txt",
        "run_frame10.txt",
        "run_frame11.txt",
        "run_frame12.txt",
        "run_frame13.txt",
        "run_frame14.txt",
        "run_frame15.txt",
        "run_frame16.txt"
    },
    "WolfTexture.png", "default.frag")
{
    SetPosition({-5,-1.5,-0.5});
    SetScale({0.1,0.1,0.1});
    SetRotation({0,90,0});
}

void Player::PlayerInteracted(PlayerInteraction playerInteraction) {
    if (playerInteraction == PlayerInteraction::Barrier) {
        state = PlayerState::GameOver;

        std::cout << "Player Interacted Barrier" << std::endl;
    }
}

void Player::Update(double dTime)
{
    AnimObject::Update(dTime);

    if (state != PlayerState::GameOver) {}
    SetPosition(GetPosition() + glm::vec3(dTime * 4, 0, 0));
}
