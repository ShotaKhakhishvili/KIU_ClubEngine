#include "Sublevel.h"

std::vector<std::vector<Placeholderinfo>> Sublevel::subLevels{
        {
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Barrier, {0,0,0}},
            {ObjectType::SABook, ObjectType::Barrier, ObjectType::Barrier, {5,0,0}},
            {ObjectType::Barrier, ObjectType::Barrier, ObjectType::SABook, {10,0,0}}
        },
        {
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {3,0,0}},
            {ObjectType::Empty, ObjectType::Barrier, ObjectType::Empty, {5,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {5,2,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {7,0,0}}
        },
    {
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {3,0,0}},
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Barrier, {5,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {5,2,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {7,0,0}}
        },
    {
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {0,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {2.5,0,0}},
            {ObjectType::Empty, ObjectType::Barrier, ObjectType::Empty, {5,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {5,2,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {7.5,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {10,0,0}}
        },
    {
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {0,0,0}},
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Barrier, {2.5,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {2.5,2,0}},
            {ObjectType::SABook, ObjectType::Barrier, ObjectType::SABook, {5,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {5,2,0}},
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Barrier, {7.5,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {7.5,2,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {10,0,0}}
        },
        {
            {ObjectType::SABook, ObjectType::Empty, ObjectType::Empty, {0,0,0}},
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Empty, {2.5,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {2.5,2,0}},
            {ObjectType::SABook, ObjectType::Barrier, ObjectType::SABook, {5,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {5,2,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Barrier, {7.5,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {7.5,2,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {10,0,0}}
        }
    };

Sublevel::Sublevel(std::vector<Placeholderinfo> infos, Player* player, glm::vec3 position) {
    for (int i = 0; i < infos.size(); i++) {
        Placeholder* newPlaceHolder = new Placeholder(infos[i].a, infos[i].b, infos[i].c, infos[i].position, player);
        Placeholders.push_back(newPlaceHolder);
    }
}

Sublevel::~Sublevel() {
    Placeholders.clear();
}


