#pragma once

#include <Core/Types/CoreTypes.h>

namespace CE
{

enum class ETriggerEvent : uint8
{
    Started,
    Ongoing,
    Triggered,
    Completed,
    Canceled
};

}
