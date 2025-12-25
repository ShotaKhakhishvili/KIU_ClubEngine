#include "Barrier.h"

Barrier::Barrier(Player* player)
    : PlayerDetector("Cube.txt", "bookText.png", "default.frag", player, PlayerInteraction::Barrier)
{

}
