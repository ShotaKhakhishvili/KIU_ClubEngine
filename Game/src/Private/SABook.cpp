#include "SABook.h"

#include "Sublevel.h"


void SABook::Update(double dTime)
{
    PlayerDetector::Update(dTime);

    moveProgress += dTime;

    for (auto& [id, inst] : objInsts)
    {
        glm::vec3 pos = inst.position;
        pos.y += glm::sin(moveProgress * movementSpeed) * 0.0025f;
        inst.position = pos;

        inst.rotation.y += rotationSpeed * dTime;
        inst.matValuesChanged = true;
    }
}


SABook::SABook(Player* player) : PlayerDetector("SA_Book.txt", "bookText.png", "default.frag", player, PlayerInteraction::SABook)
{
}
