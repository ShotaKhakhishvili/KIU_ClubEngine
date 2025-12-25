#include "BarrierA.h"

BarrierA::BarrierA(Player *player) : Barrier("A.txt", "orange.png", player, PlayerInteraction::Barrier)
{

}

unsigned int BarrierA::AddInstance(glm::vec3 position) {
    unsigned int newIndex = PlayerDetector::AddInstance(position);

    SetScale(newIndex, {1.5, 1.5,1.5});
    SetRotation(newIndex, {0, -90, 0});
    return newIndex;
}