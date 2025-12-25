#include "SABook.h"

#include "Sublevel.h"


void SABook::Update(double dTime)
{
    PlayerDetector::Update(dTime);

    moveProgress += dTime;

    for (auto& [id, inst] : objInsts)
    {
        inst.position.y =
            baseYs.at(id) + glm::sin(moveProgress * movementSpeed) * 0.25f;

        inst.rotation.y += rotationSpeed * dTime;
        inst.matValuesChanged = true;
    }
}

SABook::SABook(Player* player) : PlayerDetector("SA_Book.txt", "bookText.png", "default.frag", player, PlayerInteraction::SABook)
{
}

unsigned int SABook::AddInstance(glm::vec3 position)
{
    unsigned int id = PlayerDetector::AddInstance(position);
    baseYs.emplace(id, position.y);
    return id;
}
