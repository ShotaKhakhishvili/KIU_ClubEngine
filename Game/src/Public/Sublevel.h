#pragma once
#include <vector>

#include "Placeholder.h"
#include "glm/vec3.hpp"
#include "Player.h"

struct Placeholderinfo {
    ObjectType a;
    ObjectType b;
    ObjectType c;
    glm::vec3 position;
};

class Sublevel {

    std::vector<Placeholder*> Placeholders;

    public:

    Sublevel(std::vector<Placeholderinfo> infos, Player* player);

    ~Sublevel();
};
