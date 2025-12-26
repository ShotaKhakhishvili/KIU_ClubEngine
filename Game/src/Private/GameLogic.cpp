#include "GameLogic.h"

#include <algorithm>
#include <random>
#include <vector>

#include "World.h"
#include "Player.h"
#include "Sublevel.h"

#include "SABook.h"
#include "BarrierA.h"
#include "BarrierShuttle.h"
#include "BarrierShuttleLit.h"
#include "BarrierBoard.h"
#include "Audio.h"
#include "Path.h"

// ===== static pools =====
SABook*  GameLogic::book         = nullptr;
Barrier* GameLogic::barrierA     = nullptr;
Barrier* GameLogic::barrierBoard = nullptr;
Barrier* GameLogic::shuttle      = nullptr;
Barrier* GameLogic::shuttleLit   = nullptr;
Barrier* GameLogic::board        = nullptr;

GameLogic::GameLogic(Player* player)
{
    Audio::PlayLoop(
        Path::Sound("night-sounds-loop.mp3"),
        0.7f
    );

    this->player = player;

    // Pools are created ONCE and then reused across levels/restarts.
    book         = World::CreateActor<SABook>(player);
    barrierA     = World::CreateActor<BarrierA>(player);
    barrierBoard = World::CreateActor<BarrierBoard>(player);
    shuttle      = World::CreateActor<BarrierShuttle>(player);
    shuttleLit   = World::CreateActor<BarrierShuttleLit>(player);
    board        = World::CreateActor<BarrierBoard>(player);

    // First startup spawn
    MakeNewSubLevel(0);
    MakeNewSubLevel(1);

    // Fill ahead on first start too (optional, but makes it consistent)
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

GameLogic::~GameLogic()
{
    for (Sublevel* s : subLevels)
        delete s;

    subLevels.clear();

    // Pools are owned by World; don't delete them here.
    book = nullptr;
    barrierA = nullptr;
    barrierBoard = nullptr;
    shuttle = nullptr;
    shuttleLit = nullptr;
    board = nullptr;
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
                if (!s) return true;

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

    // NOTE: Your code used 1..size-1 (skipping template 0). Keeping that behavior.
    if (Sublevel::subLevels.size() == 1)
        return Sublevel::subLevels[0];

    std::uniform_int_distribution<std::size_t> dist(1, Sublevel::subLevels.size() - 1);
    return Sublevel::subLevels[dist(rng)];
}

void GameLogic::MakeNewSubLevel(unsigned int levelIdx)
{
    if (Sublevel::subLevels.empty())
        return;

    if (levelIdx >= Sublevel::subLevels.size())
        levelIdx = 0;

    auto* newLevel = new Sublevel(
        Sublevel::subLevels[levelIdx],
        glm::vec3(furthestSubLevel, 0, 0)
    );

    furthestSubLevel += newLevel->size + distBetweenLevels;
    subLevels.push_back(newLevel);
}

void GameLogic::OnRestart()
{
    for (Sublevel* s : subLevels)
        delete s;

    subLevels.clear();

    if (book)         book->ClearInstances();
    if (barrierA)     barrierA->ClearInstances();
    if (barrierBoard) barrierBoard->ClearInstances();
    if (shuttle)      shuttle->ClearInstances();
    if (shuttleLit)   shuttleLit->ClearInstances();
    if (board)        board->ClearInstances();

    furthestSubLevel = player->GetPosition().x;

    spawned = 0.0;
    lastTime = 0.0;

    MakeNewSubLevel(0);
    MakeNewSubLevel(1);
}


