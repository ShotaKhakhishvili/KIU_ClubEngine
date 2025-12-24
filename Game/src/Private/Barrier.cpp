#include "Barrier.h"

Barrier::Barrier(Player* player, glm::vec3 pos)
    : PlayerDetector("SA_Book.txt", "bookText.png", "default.frag", player, PlayerInteraction::Barrier)
{
    SetPosition(pos);
}
