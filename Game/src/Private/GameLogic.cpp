#include "GameLogic.h"
#include <vector>     // std::vector
#include <memory>     // std::unique_ptr
#include <algorithm> // std::remove_if
#include <random>
#include "World.h"
#include "SABook.h"

SABook* GameLogic::book = nullptr;
Barrier* GameLogic::barrier = nullptr;

GameLogic::GameLogic(Player* player) {
    this->player =player;

    book = World::CreateActor<SABook>(player);
    barrier = World::CreateActor<Barrier>(player);
}

void GameLogic::Update(double dTime)
{
    std::vector<Sublevel*> toDelete;

    subLevels.erase(
        std::remove_if(
            subLevels.begin(),
            subLevels.end(),
            [&](Sublevel* s)
            {
                if ((s->position.x - player->GetPosition().x) < cullDistance)
                {
                    toDelete.push_back(s);
                    return true;
                }
                return false;
            }),
        subLevels.end()
    );

    for (Sublevel* s : toDelete)
        delete s;

    while (furthestSubLevel < player->GetPosition().x + forwardCullDistance)
    {
        if (Sublevel::subLevels.empty())
            break;

        auto* newLevel = new Sublevel(
            GetRandomSubLevel(),
            glm::vec3(furthestSubLevel, 0, 0)
        );

        furthestSubLevel += newLevel->size + distBetweenLevels;
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

GameLogic::~GameLogic()
{
    for (Sublevel* s : subLevels)
        delete s;

    subLevels.clear();

    book = nullptr;
    barrier = nullptr;
}