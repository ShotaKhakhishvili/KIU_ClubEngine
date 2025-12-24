#include "GameLogic.h"
#include <vector>     // std::vector
#include <memory>     // std::unique_ptr
#include <algorithm> // std::remove_if
#include <random>

GameLogic::GameLogic(Player* player) {
    this->player =player;
}

void GameLogic::Update(double dTime)
{
    subLevels.erase(
        std::remove_if(
            subLevels.begin(),
            subLevels.end(),
            [&](Sublevel* s)
            {
                if (s && (s->position.x - player->GetPosition().x) < cullDistance)
                {
                    delete s;
                    return true;
                }
                return false;
            }),
        subLevels.end()
    );


    while (furthestSubLevel < player->GetPosition().x + forwardCullDistance) {
        auto* newLevel = new Sublevel(GetRandomSubLevel(), player, glm::vec3(furthestSubLevel, 0, 0));
        furthestSubLevel += newLevel->size + 1;
        subLevels.push_back(newLevel);
    }
}

const std::vector<Placeholderinfo>& GameLogic::GetRandomSubLevel()
{
    static std::mt19937 rng{ std::random_device{}() };

    if (Sublevel::subLevels.empty())
        throw std::runtime_error("No sublevel templates available");

    std::uniform_int_distribution<std::size_t> dist(
        0, Sublevel::subLevels.size() - 1
    );

    return Sublevel::subLevels[dist(rng)];
}
