#include "Sublevel.h"

std::vector<std::vector<Placeholderinfo>> Sublevel::subLevels{
    {
            {{ObjectType::ShuttleLit, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {0,0,0}, 10},
            {{ObjectType::Shuttle, 6, 3}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {6,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::ShuttleLit, 0, 1}, {6,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 2, 4}, {ObjectType::Empty, 0, 1}, {18,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 6, 2}, {ObjectType::Empty, 0, 1}, {26,0,1}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 2, 2}, {28,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABook, 0, 1}, {ObjectType::Empty, 0, 1}, {34,0,0}},

            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {40,0,0}}
        },
    {
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::ShuttleLit, 0, 1}, {0,0,0}, 10},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Shuttle, 6, 3}, {0,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 2, 3}, {ObjectType::Empty, 0, 1}, {5,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::ShuttleLit, 0, 1}, {ObjectType::Empty, 0, 1}, {15,0,0}},
            {{ObjectType::SABookSequence, 12, 2}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {11,0,1}},
            {{ObjectType::SABookSequence, 2, 5}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {13,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 2, 2}, {ObjectType::Empty, 0, 1}, {25,0,0}},

            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {33,0,0}}
        },
    {
            {{ObjectType::SABookSequence, 2, 6}, {ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 10, 2}, {0,0,0}, 10},
            {{ObjectType::Empty, 0, 1}, {ObjectType::ShuttleLit, 0, 1}, {ObjectType::Empty, 0, 1}, {0,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Shuttle, 6, 2}, {ObjectType::Empty, 0, 1}, {6,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 6, 2}, {2,0.6,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 2, 2}, {4,1.2,0}},
            {{ObjectType::BarrierBoard, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::BarrierA, 0, 1}, {5,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::ShuttleLit, 0, 1}, {21,0,0}},

            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {31,0,0}},
        },
        {
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 10, 2}, {0,0,0}, 10},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 6, 2}, {2,0.6,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::SABookSequence, 2, 2}, {4,1.2,0}},
            {{ObjectType::SABookSequence, 10, 2}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {2,0,0}},
            {{ObjectType::SABookSequence, 6, 2}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {4,0.6,0}},
            {{ObjectType::SABookSequence, 2, 2}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {6,1.2,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::ShuttleLit, 0, 1}, {ObjectType::BarrierA, 0, 1}, {5,0,0}},
            {{ObjectType::BarrierA, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {7,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Shuttle, 0, 1}, {ObjectType::Empty, 0, 1}, {11,0,0}},
            {{ObjectType::BarrierBoard, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::BarrierA, 0, 1}, {20,0,0}},

            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {28,0,0}}
        },
        {
            {{ObjectType::ShuttleLit, 0, 1}, {ObjectType::SABookSequence, 2, 5}, {ObjectType::ShuttleLit, 0, 1}, {0,0,0}, 10},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Shuttle, 6, 4}, {6,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::ShuttleLit, 0, 1}, {ObjectType::Empty, 0, 1}, {14,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::Shuttle, 6, 2}, {ObjectType::Empty, 0, 1}, {20,0,0}},
            {{ObjectType::BarrierA, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {22,0,0}},
            {{ObjectType::SABookSequence, 2, 4}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {26,0,0}},
            {{ObjectType::ShuttleLit, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::ShuttleLit, 0, 1}, {37,0,0}},
            {{ObjectType::Shuttle, 6, 2}, {ObjectType::Empty, 0, 1}, {ObjectType::Shuttle, 6, 2}, {43,0,0}},
            {{ObjectType::Empty, 0, 1}, {ObjectType::BarrierBoard, 0, 1}, {ObjectType::Empty, 0, 1}, {46,0,0}},
            {{ObjectType::SABookSequence, 10, 2}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {16,0,0}},
            {{ObjectType::SABookSequence, 6, 2}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {18,0.6,0}},
            {{ObjectType::SABookSequence, 2, 2}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {20,1.2,0}},

            {{ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {ObjectType::Empty, 0, 1}, {59,0,0}}
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



