#pragma once
#include <vector>

#include "Placeholder.h"
#include "glm/vec3.hpp"
#include "Player.h"

struct Placeholderinfo {
    ObjectWrapper a;
    ObjectWrapper b;
    ObjectWrapper c;
    glm::vec3 position;
    double size;
};

class Sublevel {

    std::vector<Placeholder*> Placeholders;

    public:
    static std::vector<std::vector<Placeholderinfo>> subLevels;
    Sublevel(std::vector<Placeholderinfo> infos, glm::vec3 position);
    glm::vec3 position{};
    double size{};
    ~Sublevel();
};
