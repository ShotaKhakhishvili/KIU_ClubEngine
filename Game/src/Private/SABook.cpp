#include "SABook.h"


void SABook::Update(double dTime) {
    Object::Update(dTime);

    SetPosition(GetPosition() + glm::vec3(0, dTime, 0));
}

SABook::SABook() : Object("Cube.txt", "1.png", "default.frag")
{

}
