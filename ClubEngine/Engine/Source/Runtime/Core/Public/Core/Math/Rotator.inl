#pragma once

namespace CE
{

template<std::floating_point T>
TQuat<T> TRotator<T>::ToQuat() const
{
    return TQuat<T>::FromEulerDegrees({ pitch, yaw, roll });
}

template<std::floating_point T>
TRotator<T> TRotator<T>::FromQuat(const TQuat<T>& q)
{
    const TVector<T> e = q.ToEulerDegrees();
    return { e.x, e.y, e.z };
}

}