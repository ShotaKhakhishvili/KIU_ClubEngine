#include "Sublevel.h"

std::vector<std::vector<Placeholderinfo>> Sublevel::subLevels{
        {
            {{ObjectType::Barrier, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Barrier, 0, 1}, {0,0,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Barrier, 0, 1}, {ObjectType::Barrier, 0, 1}, {10,0,0}},
            {{ObjectType::Barrier, 0, 1}, {ObjectType::Barrier, 0, 1}, {ObjectType::SABook, 0, 1}, {20,0,0}}
        },
        {
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {6,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Barrier, 0, 1}, {ObjectType::Empty, 0, 1}, {10,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {10,2,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {14,0,0}}
        },
    {
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {6,0,0}},
            {{ObjectType::Barrier, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Barrier, 0, 1}, {10,0,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {10,2,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {14,0,0}}
        },
    {
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {0,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {5,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Barrier, 0, 1}, {ObjectType::Empty, 0, 1}, {10,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {10,2,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {15,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {20,0,0}}
        },
    {
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {0,0,0}},
            {{ObjectType::Barrier, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Barrier, 0, 1}, {5,0,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {5,2,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Barrier, 0, 1}, {ObjectType::SABook, 0, 1}, {10,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {10,2,0}},
            {{ObjectType::Barrier, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Barrier, 0, 1}, {15,0,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {15,2,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {20,0,0}}
        },
        {
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {0,0,0}},
            {{ObjectType::Barrier, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {5,0,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {5,2,0}},
            {{ObjectType::SABook, 0, 1}, {ObjectType::Barrier, 0, 1}, {ObjectType::SABook, 0, 1}, {10,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {10,2,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Barrier, 0, 1}, {15,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {15,2,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {20,0,0}}
        }
    };

Sublevel::Sublevel(std::vector<Placeholderinfo> infos, glm::vec3 position) {
    for (int i = 0; i < infos.size(); i++) {
        auto* newPlaceHolder = new Placeholder(infos[i].a,infos[i].b, infos[i].c,
            position + infos[i].position);
        Placeholders.push_back(newPlaceHolder);
    }
    this->size = infos.back().position.x;
    this->position = position;
}

Sublevel::~Sublevel() {
    for (auto* p : Placeholders) delete p;
    Placeholders.clear();
}



