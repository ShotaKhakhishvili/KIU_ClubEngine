#pragma once

#include <Core/Containers/TArray.h>
#include <Core/Math/Vector.h>

#include <bitset>
#include <Core/Types/InputTypes.h>

namespace CE
{

struct FKeyPressState
{
    EKey key;
    bool pressed;   // True means pressed, false means released
};

struct FInputState
{
    std::bitset<EKey::Count>        down;
    std::bitset<EKey::Count>        pressed;
    std::bitset<EKey::Count>        released;
    FVector2F                       mouseDelta;
    float                           wheelDelta{};

    TArray<FKeyPressState>          pressReleaseList;

    bool IsDown         (EKey key) const;
    bool WasPressed     (EKey key) const;
    bool WasReleased    (EKey key) const;

    float GetKeyValue(EKey key) const;
};

}
