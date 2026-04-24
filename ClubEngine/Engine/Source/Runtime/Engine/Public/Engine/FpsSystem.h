#pragma once

#include <Engine/ISystem.h>

#include <queue>

class FpsSystem : public ISystem
{
public:
    const char* GetName() const noexcept override;
    int Update(float dt) override;

    float GetFPS()const;

private:
    std::queue<float> deltas{};
    float acc = 0;
};