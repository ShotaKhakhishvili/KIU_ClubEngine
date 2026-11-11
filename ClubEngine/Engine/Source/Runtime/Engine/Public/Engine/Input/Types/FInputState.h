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
    FVector2F                       mouseDelta;
    float                           wheelDelta{};
    FVector2D                       mousePos;

    TArray<FKeyPressState>          pressReleaseList;

    bool IsDown         (EKey key) const;

    float GetKeyValue(EKey key) const;
};

}
