#pragma once

#include <Core/Math/Vector.h>
#include <Engine/Input/Types/EInputActionValueType.h>

namespace CE
{

struct FInputActionValue
{
    FVectorF value;
    EInputActionValueType type;
};

}
