#include "Game.h"
#include "SABook.h"
#include "Barrier.h"
#include "Placeholder.h"

Placeholder::Placeholder(ObjectType a, ObjectType b, ObjectType c, glm::vec3 position, Player* player){

    if (a == ObjectType::SABook) {
        LeftObject = World::CreateActor<SABook>(player, position + glm::vec3(0, 0, -1));
    } else if (a == ObjectType::Barrier) {
        // LeftObject = World::CreateActor<Barrier>(player, position + glm::vec3(0, 0, 0));
    }

    if (b == ObjectType::SABook) {
        MiddleObject = World::CreateActor<SABook>(player, position + glm::vec3(0, 0, 0));
    } else if (b == ObjectType::Barrier) {
        // MiddleObject = World::CreateActor<Barrier>(player, position + glm::vec3(0, 0, 0));
    }

    if (c == ObjectType::SABook) {
        RightObject = World::CreateActor<SABook>(player, position + glm::vec3(0, 0, 1));
    } else if (c == ObjectType::Barrier) {
        // RightObject = World::CreateActor<Barrier>(player, position + glm::vec3(0, 0, 0));
    }
}

Placeholder::~Placeholder() {
    World::DestroyActor(LeftObject);
    World::DestroyActor(MiddleObject);
    World::DestroyActor(RightObject);
}

