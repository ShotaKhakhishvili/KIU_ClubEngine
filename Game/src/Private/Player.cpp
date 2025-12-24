#include "Player.h"

Player::Player() :
AnimObject("my_animation.000",62,"WolfTexture.png", "default.frag")
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

}
