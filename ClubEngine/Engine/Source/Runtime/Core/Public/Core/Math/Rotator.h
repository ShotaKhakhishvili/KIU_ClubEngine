#pragma once

#include <Core/Math/Quat.h>

namespace CE
{

template<std::floating_point T>
struct TRotator
{
    T pitch = 0, yaw = 0, roll = 0;

    TQuat<T> ToQuat() const;
    static TRotator FromQuat(const TQuat<T>& q);
};

using FRotator  = TRotator<double>;
using FRotatorF = TRotator<float>;

}

#include "Rotator.inl"