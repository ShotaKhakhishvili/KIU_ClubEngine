// ─── AI-Assisted Code ────────────────────────────────────────────────────────
//
//  This file was generated with the assistance of Claude (Anthropic).
//  Reviewed and approved by: Shota Khakhishvili
//  Date: 2026-05-12
//
//  If you modify this file, remove or update this notice accordingly.
//
// ─────────────────────────────────────────────────────────────────────────────

#pragma once

#include <Core/Math/MathCore.h>
#include <Core/Math/Vector.h>

#include <concepts>

namespace CE
{

template<std::floating_point T>
struct TMatrix4x4;

template<std::floating_point T>
struct TQuat
{
    T x{}, y{}, z{}, w{ T(1) };

    // ── Construction ──────────────────────────────────────────────────────────

    constexpr TQuat() = default;
    constexpr TQuat(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    template<std::floating_point U>
    explicit constexpr TQuat(const TQuat<U>& o)
        : x(T(o.x)), y(T(o.y)), z(T(o.z)), w(T(o.w)) {}

    // ── Static factories ──────────────────────────────────────────────────────

    static constexpr TQuat  Identity();
    static           TQuat  FromAxisAngle(TVector<T> axis, T angle);
    static           TQuat  FromEuler(T pitch, T yaw, T roll);
    static           TQuat  FromEulerDegrees(TVector<T> eulerDeg);
    static           TQuat  FromToRotation(TVector<T> from, TVector<T> to);
    static           TQuat  FromMatrix(const TMatrix4x4<T>& m);

    // ── Conversion ────────────────────────────────────────────────────────────

    TMatrix4x4<T>   ToMatrix()          const;
    TVector<T>      ToEuler()           const;
    TVector<T>      ToEulerDegrees()    const;

    // ── Basis directions ──────────────────────────────────────────────────────

    TVector<T>  GetForward()    const;
    TVector<T>  GetBack()       const;
    TVector<T>  GetRight()      const;
    TVector<T>  GetLeft()       const;
    TVector<T>  GetUp()         const;
    TVector<T>  GetDown()       const;

    // ── Operators ─────────────────────────────────────────────────────────────

    constexpr TQuat     operator    *   (const TQuat& r)        const;
    constexpr TQuat&    operator    *=  (const TQuat& r);
    constexpr TVector<T> operator   *   (TVector<T> v)          const;
    constexpr TQuat     operator    -   ()                      const;
    constexpr bool      operator    ==  (const TQuat& r)        const;
    constexpr bool      operator    !=  (const TQuat& r)        const;
};

// ─── Free functions ───────────────────────────────────────────────────────────

template<std::floating_point T> constexpr T         Dot         (const TQuat<T>& a, const TQuat<T>& b);
template<std::floating_point T> inline    T         Length      (const TQuat<T>& q);
template<std::floating_point T> inline    TQuat<T>  Normalize   (const TQuat<T>& q);
template<std::floating_point T> constexpr TQuat<T>  Conjugate   (const TQuat<T>& q);
template<std::floating_point T> inline    TQuat<T>  Inverse     (const TQuat<T>& q);
template<std::floating_point T>           TQuat<T>  Slerp       (TQuat<T> a, TQuat<T> b, T t);
template<std::floating_point T>           TQuat<T>  Nlerp       (TQuat<T> a, TQuat<T> b, T t);
template<std::floating_point T> constexpr bool      NearlyEqual (const TQuat<T>& a, const TQuat<T>& b, T tolerance = SMALL_NUMBER<T>);

// ─── Type aliases ─────────────────────────────────────────────────────────────

using FQuat  = TQuat<double>;
using FQuatF = TQuat<float>;

} // namespace CE

#include <Core/Math/Quat.inl>