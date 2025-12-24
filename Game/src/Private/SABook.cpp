#include "SABook.h"


void SABook::Update(double dTime) {
    PlayerDetector::Update(dTime);

    SetPosition(GetPosition() - glm::vec3(0, glm::sin(moveProgress * movementSpeed) * 0.25, 0));
    moveProgress += dTime;
    SetPosition(GetPosition() + glm::vec3(0, glm::sin(moveProgress * movementSpeed) * 0.25, 0));

    SetRotation(GetRotation() + glm::vec3(0, rotationSpeed * dTime,0 ));
}

SABook::SABook(Player* player, glm::vec3 pos) : PlayerDetector("SA_Book.txt", "bookText.png", "default.frag", player, PlayerInteraction::SABook)
{
    SetPosition(pos);
}
