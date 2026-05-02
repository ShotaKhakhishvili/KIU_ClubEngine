#include <Engine/Systems/FpsSystem.h>

namespace CE
{

const char* FpsSystem::GetName() const noexcept
{
    return "FPS System";
}

int FpsSystem::Update(float dt)
{
    acc += dt;
    deltas.push(dt);
    
    while(!deltas.empty())
    {
        const float front = deltas.front();

        if(acc - front <= 1.0f)
            break;
        
        acc -= front;
        deltas.pop();
    }

    return 0;
}

float FpsSystem::GetFPS() const
{
    if(acc <= 0)
        return 0.0f;

    return static_cast<float>(deltas.size()) / acc;
}

}