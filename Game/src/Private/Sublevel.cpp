#include "Sublevel.h"

Sublevel::Sublevel(std::vector<Placeholderinfo> infos, Player* player) {
    for (int i = 0; i < infos.size(); i++) {
        Placeholder* newPlaceHolder = new Placeholder(infos[i].a, infos[i].b, infos[i].c, infos[i].position, player);
        Placeholders.push_back(newPlaceHolder);
    }
}

Sublevel::~Sublevel() {
    Placeholders.clear();
}


