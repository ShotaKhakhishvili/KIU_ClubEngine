#include "BarrierBoard.h"

BarrierBoard::BarrierBoard(Player *player) : Barrier("board.txt", "boardTexture.png", player, PlayerInteraction::Barrier)
{

}

unsigned int BarrierBoard::AddInstance(glm::vec3 position) {
    unsigned int newIndex = PlayerDetector::AddInstance(position);

    SetScale(newIndex, {2.5, 2.5,2.5});
    SetRotation(newIndex, {0, -90, 0});
    return newIndex;
}