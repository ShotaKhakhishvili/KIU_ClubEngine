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
    static std::vector<std::vector<Placeholderinfo>> subLevels;

    public:

    Sublevel(std::vector<Placeholderinfo> infos, Player* player, glm::vec3 position);
    glm::vec3 position{};
    double size{};
    ~Sublevel();
};
