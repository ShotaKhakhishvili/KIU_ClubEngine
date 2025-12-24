#include "Barrier.h"

Barrier::Barrier(Player* player, glm::vec3 pos)
    : PlayerDetector("Cube.txt", "bookText.png", "default.frag", player, PlayerInteraction::Barrier)
{
    SetPosition(pos);
    SetScale({1,3,3});
}
