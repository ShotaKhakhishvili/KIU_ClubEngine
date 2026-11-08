#pragma once

#include <Core/Math/Vector.h>

#include <bitset>
#include <Core/Types/InputTypes.h>

namespace CE
{

struct FInputState
{
    std::bitset<EKey::Count>    down;
    std::bitset<EKey::Count>    pressed;
    std::bitset<EKey::Count>    released;
    FVector2F                   mouseDelta;
    float                       wheelDelta{};

    bool IsDown         (EKey key) const;
    bool WasPressed     (EKey key) const;
    bool WasReleased    (EKey key) const;

    float GetKeyValue(EKey key) const;
};

}
