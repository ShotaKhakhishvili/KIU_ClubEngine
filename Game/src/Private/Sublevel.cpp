#include "Sublevel.h"

std::vector<std::vector<Placeholderinfo>> Sublevel::subLevels{
        {
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Barrier, {0,0,0}},
            {ObjectType::SABook, ObjectType::Barrier, ObjectType::Barrier, {10,0,0}},
            {ObjectType::Barrier, ObjectType::Barrier, ObjectType::SABook, {20,0,0}}
        },
        {
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {6,0,0}},
            {ObjectType::Empty, ObjectType::Barrier, ObjectType::Empty, {10,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {10,2,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {14,0,0}}
        },
    {
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {6,0,0}},
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Barrier, {10,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {10,2,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {14,0,0}}
        },
    {
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {0,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {5,0,0}},
            {ObjectType::Empty, ObjectType::Barrier, ObjectType::Empty, {10,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {10,2,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {15,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {20,0,0}}
        },
    {
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {0,0,0}},
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Barrier, {5,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {5,2,0}},
            {ObjectType::SABook, ObjectType::Barrier, ObjectType::SABook, {10,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {10,2,0}},
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Barrier, {15,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {15,2,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::SABook, {20,0,0}}
        },
        {
            {ObjectType::SABook, ObjectType::Empty, ObjectType::Empty, {0,0,0}},
            {ObjectType::Barrier, ObjectType::SABook, ObjectType::Empty, {5,0,0}},
            {ObjectType::SABook, ObjectType::Empty, ObjectType::Empty, {5,2,0}},
            {ObjectType::SABook, ObjectType::Barrier, ObjectType::SABook, {10,0,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Empty, {10,2,0}},
            {ObjectType::Empty, ObjectType::SABook, ObjectType::Barrier, {15,0,0}},
            {ObjectType::Empty, ObjectType::Empty, ObjectType::SABook, {15,2,0}},
            {ObjectType::Empty, ObjectType::Empty, ObjectType::SABook, {20,0,0}}
        }
    };

Sublevel::Sublevel(std::vector<Placeholderinfo> infos, glm::vec3 position) {
    for (int i = 0; i < infos.size(); i++) {
        auto* newPlaceHolder = new Placeholder(infos[i].a, infos[i].b, infos[i].c, position + infos[i].position);
        Placeholders.push_back(newPlaceHolder);
    }
    this->size = infos.back().position.x;
    this->position = position;
}

Sublevel::~Sublevel() {
    for (auto* p : Placeholders) delete p;
    Placeholders.clear();
}



