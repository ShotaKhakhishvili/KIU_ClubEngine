// ─── AI-Assisted Code ─────────────────────────────────────────────────────────
//
//  This file was generated with the assistance of Claude (Anthropic).
//  Reviewed and approved by: Shota Khakhishvili
//  Date: 2026-05-12
//
//  If you modify this file, remove or update this notice accordingly.
//
// ─────────────────────────────────────────────────────────────────────────────

#pragma once

#include <Core/Math/Matrix.h>  // full definition needed for ToMatrix / FromMatrix
#include <cmath>

namespace CE
{

// ─── Static factories ─────────────────────────────────────────────────────────

template<std::floating_point T>
constexpr TQuat<T> TQuat<T>::Identity()
{
    return { T(0), T(0), T(0), T(1) };
}

template<std::floating_point T>
TQuat<T> TQuat<T>::FromAxisAngle(TVector<T> axis, T angle)
{
    const T half = angle * T(0.5);
    const T s    = std::sin(half);
    return { axis.x * s, axis.y * s, axis.z * s, std::cos(half) };
}

template<std::floating_point T>
TQuat<T> TQuat<T>::FromEuler(T pitch, T yaw, T roll)
{
    const T hp = pitch * T(0.5);
    const T hy = yaw   * T(0.5);
    const T hr = roll  * T(0.5);

    const T cp = std::cos(hp), sp = std::sin(hp);
    const T cy = std::cos(hy), sy = std::sin(hy);
    const T cr = std::cos(hr), sr = std::sin(hr);

    return {
        sp*cy*cr + cp*sy*sr,
        cp*sy*cr - sp*cy*sr,
        cp*cy*sr + sp*sy*cr,
        cp*cy*cr - sp*sy*sr
    };
}

template<std::floating_point T>
TQuat<T> TQuat<T>::FromEulerDegrees(TVector<T> eulerDeg)
{
    return FromEuler(
        ToRadians(eulerDeg.x),
        ToRadians(eulerDeg.y),
        ToRadians(eulerDeg.z));
}

template<std::floating_point T>
TQuat<T> TQuat<T>::FromToRotation(TVector<T> from, TVector<T> to)
{
    const T d = Dot(from, to);

    if (d < T(-1) + SMALL_NUMBER<T>)
    {
        TVector<T> perp = Cross(TVector<T>{ T(1), T(0), T(0) }, from);
        if (Dot(perp, perp) < SMALL_NUMBER<T>)
            perp = Cross(TVector<T>{ T(0), T(1), T(0) }, from);
        return FromAxisAngle(Normalize(perp), PI<T>);
    }

    const TVector<T> c = Cross(from, to);
    return Normalize(TQuat{ c.x, c.y, c.z, T(1) + d });
}

template<std::floating_point T>
TQuat<T> TQuat<T>::FromMatrix(const TMatrix4x4<T>& m)
{
    const T trace = m.Get(0,0) + m.Get(1,1) + m.Get(2,2);

    TQuat<T> q;
    if (trace > T(0))
    {
        const T s = T(0.5) / std::sqrt(trace + T(1));
        q.w = T(0.25) / s;
        q.x = (m.Get(2,1) - m.Get(1,2)) * s;
        q.y = (m.Get(0,2) - m.Get(2,0)) * s;
        q.z = (m.Get(1,0) - m.Get(0,1)) * s;
    }
    else if (m.Get(0,0) > m.Get(1,1) && m.Get(0,0) > m.Get(2,2))
    {
        const T s = T(2) * std::sqrt(T(1) + m.Get(0,0) - m.Get(1,1) - m.Get(2,2));
        q.w = (m.Get(2,1) - m.Get(1,2)) / s;
        q.x = T(0.25) * s;
        q.y = (m.Get(0,1) + m.Get(1,0)) / s;
        q.z = (m.Get(0,2) + m.Get(2,0)) / s;
    }
    else if (m.Get(1,1) > m.Get(2,2))
    {
        const T s = T(2) * std::sqrt(T(1) + m.Get(1,1) - m.Get(0,0) - m.Get(2,2));
        q.w = (m.Get(0,2) - m.Get(2,0)) / s;
        q.x = (m.Get(0,1) + m.Get(1,0)) / s;
        q.y = T(0.25) * s;
        q.z = (m.Get(1,2) + m.Get(2,1)) / s;
    }
    else
    {
        const T s = T(2) * std::sqrt(T(1) + m.Get(2,2) - m.Get(0,0) - m.Get(1,1));
        q.w = (m.Get(1,0) - m.Get(0,1)) / s;
        q.x = (m.Get(0,2) + m.Get(2,0)) / s;
        q.y = (m.Get(1,2) + m.Get(2,1)) / s;
        q.z = T(0.25) * s;
    }
    return q;
}

// ─── Conversion ───────────────────────────────────────────────────────────────

template<std::floating_point T>
TMatrix4x4<T> TQuat<T>::ToMatrix() const
{
    const T xx = x*x, yy = y*y, zz = z*z;
    const T xy = x*y, xz = x*z, yz = y*z;
    const T wx = w*x, wy = w*y, wz = w*z;

    return {
        { T(1)-T(2)*(yy+zz),    T(2)*(xy+wz),       T(2)*(xz-wy),       T(0) },
        { T(2)*(xy-wz),         T(1)-T(2)*(xx+zz),  T(2)*(yz+wx),       T(0) },
        { T(2)*(xz+wy),         T(2)*(yz-wx),       T(1)-T(2)*(xx+yy),  T(0) },
        { T(0),                 T(0),               T(0),               T(1) }
    };
}

template<std::floating_point T>
TVector<T> TQuat<T>::ToEuler() const
{
    const T sinr_cosp = T(2) * (w*x + y*z);
    const T cosr_cosp = T(1) - T(2) * (x*x + y*y);
    const T roll  = std::atan2(sinr_cosp, cosr_cosp);

    const T sinp  = T(2) * (w*y - z*x);
    const T pitch = (std::abs(sinp) >= T(1))
                  ? std::copysign(HALF_PI<T>, sinp)
                  : std::asin(sinp);

    const T siny_cosp = T(2) * (w*z + x*y);
    const T cosy_cosp = T(1) - T(2) * (y*y + z*z);
    const T yaw   = std::atan2(siny_cosp, cosy_cosp);

    return { pitch, yaw, roll };
}

template<std::floating_point T>
TVector<T> TQuat<T>::ToEulerDegrees() const
{
    const TVector<T> e = ToEuler();
    return { ToDegrees(e.x), ToDegrees(e.y), ToDegrees(e.z) };
}

// ─── Basis directions ─────────────────────────────────────────────────────────

template<std::floating_point T>
TVector<T> TQuat<T>::GetForward() const
{
    return { T(2)*(x*z + w*y), T(2)*(y*z - w*x), T(1) - T(2)*(x*x + y*y) };
}

template<std::floating_point T>
TVector<T> TQuat<T>::GetBack() const { return -GetForward(); }

template<std::floating_point T>
TVector<T> TQuat<T>::GetRight() const
{
    return { T(1) - T(2)*(y*y + z*z), T(2)*(x*y + w*z), T(2)*(x*z - w*y) };
}

template<std::floating_point T>
TVector<T> TQuat<T>::GetLeft() const { return -GetRight(); }

template<std::floating_point T>
TVector<T> TQuat<T>::GetUp() const
{
    return { T(2)*(x*y - w*z), T(1) - T(2)*(x*x + z*z), T(2)*(y*z + w*x) };
}

template<std::floating_point T>
TVector<T> TQuat<T>::GetDown() const { return -GetUp(); }

// ─── Operators ────────────────────────────────────────────────────────────────

template<std::floating_point T>
constexpr TQuat<T> TQuat<T>::operator*(const TQuat<T>& r) const
{
    return {
        w*r.x + x*r.w + y*r.z - z*r.y,
        w*r.y - x*r.z + y*r.w + z*r.x,
        w*r.z + x*r.y - y*r.x + z*r.w,
        w*r.w - x*r.x - y*r.y - z*r.z
    };
}

template<std::floating_point T>
constexpr TQuat<T>& TQuat<T>::operator*=(const TQuat<T>& r)
{
    *this = *this * r;
    return *this;
}

template<std::floating_point T>
constexpr TVector<T> TQuat<T>::operator*(TVector<T> v) const
{
    const TVector<T> qv  { x, y, z };
    const TVector<T> uv  = Cross(qv, v);
    const TVector<T> uuv = Cross(qv, uv);
    return v + (uv * (T(2) * w)) + (uuv * T(2));
}

template<std::floating_point T>
constexpr TQuat<T> TQuat<T>::operator-() const { return { -x, -y, -z, -w }; }

template<std::floating_point T>
constexpr bool TQuat<T>::operator==(const TQuat<T>& r) const
{
    return x==r.x && y==r.y && z==r.z && w==r.w;
}

template<std::floating_point T>
constexpr bool TQuat<T>::operator!=(const TQuat<T>& r) const { return !(*this == r); }

// ─── Free functions ───────────────────────────────────────────────────────────

template<std::floating_point T>
constexpr T Dot(const TQuat<T>& a, const TQuat<T>& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

template<std::floating_point T>
inline T Length(const TQuat<T>& q)
{
    return std::sqrt(Dot(q, q));
}

template<std::floating_point T>
inline TQuat<T> Normalize(const TQuat<T>& q)
{
    const T len = Length(q);
    if (NearlyZero(len))
        return TQuat<T>::Identity();
    const T inv = T(1) / len;
    return { q.x*inv, q.y*inv, q.z*inv, q.w*inv };
}

template<std::floating_point T>
constexpr TQuat<T> Conjugate(const TQuat<T>& q)
{
    return { -q.x, -q.y, -q.z, q.w };
}

template<std::floating_point T>
inline TQuat<T> Inverse(const TQuat<T>& q)
{
    const T lenSq = Dot(q, q);
    if (NearlyZero(lenSq))
        return TQuat<T>::Identity();
    const T inv = T(1) / lenSq;
    return { -q.x*inv, -q.y*inv, -q.z*inv, q.w*inv };
}

template<std::floating_point T>
TQuat<T> Slerp(TQuat<T> a, TQuat<T> b, T t)
{
    T cosHalfAngle = Dot(a, b);

    if (cosHalfAngle < T(0))
    {
        b             = -b;
        cosHalfAngle  = -cosHalfAngle;
    }

    if (cosHalfAngle > T(1) - SMALL_NUMBER<T>)
        return Normalize(TQuat<T>{ a.x + t*(b.x-a.x), a.y + t*(b.y-a.y), a.z + t*(b.z-a.z), a.w + t*(b.w-a.w) });

    const T halfAngle = std::acos(cosHalfAngle);
    const T sinHalf   = std::sin(halfAngle);
    const T factorA   = std::sin((T(1) - t) * halfAngle) / sinHalf;
    const T factorB   = std::sin(t * halfAngle) / sinHalf;

    return {
        factorA*a.x + factorB*b.x,
        factorA*a.y + factorB*b.y,
        factorA*a.z + factorB*b.z,
        factorA*a.w + factorB*b.w
    };
}

template<std::floating_point T>
TQuat<T> Nlerp(TQuat<T> a, TQuat<T> b, T t)
{
    if (Dot(a, b) < T(0))
        b = -b;
    return Normalize(TQuat<T>{
        a.x + t*(b.x-a.x),
        a.y + t*(b.y-a.y),
        a.z + t*(b.z-a.z),
        a.w + t*(b.w-a.w)
    });
}

template<std::floating_point T>
constexpr bool NearlyEqual(const TQuat<T>& a, const TQuat<T>& b, T tolerance)
{
    return Dot(a, b) >= T(1) - tolerance;
}

} // namespace CE