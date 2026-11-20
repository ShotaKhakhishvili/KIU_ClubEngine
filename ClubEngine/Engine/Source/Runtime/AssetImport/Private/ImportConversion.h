#pragma once

#include <Core/Math/Quat.h>
#include <Core/Math/Vector.h>

#include <concepts>

namespace CE::ImportConversion
{

template<std::floating_point T>
constexpr TVector<T> GltfToEngine(TVector<T> v)
{
    return { v.z, -v.x, v.y };
}

template<std::floating_point T>
constexpr TQuat<T> GltfToEngine(TQuat<T> q)
{
    const TVector<T> axis = GltfToEngine(TVector<T>{ q.x, q.y, q.z });
    return { -axis.x, -axis.y, -axis.z, q.w };
}

template<std::floating_point T>
constexpr TVector<T> GltfScaleToEngine(TVector<T> s)
{
    return { s.z, s.x, s.y };
}
}