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
#include <Core/Math/Matrix4x4.h>
#include <Core/Math/Quat.h>

#include <concepts>

namespace CE
{

template<std::floating_point T>
struct TTransform
{
    TVector<T> Translation { T(0), T(0), T(0) };
    TQuat<T>   Rotation     = TQuat<T>::Identity();
    TVector<T> Scale        { T(1), T(1), T(1) };

    // ── Construction ──────────────────────────────────────────────────────────

    constexpr TTransform() = default;

    constexpr TTransform(TVector<T> translation, TQuat<T> rotation, TVector<T> scale)
        : Translation(translation), Rotation(rotation), Scale(scale) {}

    explicit constexpr TTransform(TVector<T> translation)
        : Translation(translation) {}

    template<std::floating_point U>
    explicit constexpr TTransform(const TTransform<U>& o)
        : Translation(TVector<T>(o.Translation))
        , Rotation(TQuat<T>(o.Rotation))
        , Scale(TVector<T>(o.Scale))
    {}

    // ── Static factories ──────────────────────────────────────────────────────

    static constexpr TTransform Identity();

    // ── Conversion ────────────────────────────────────────────────────────────

    TMatrix4x4<T>   ToMatrix()          const;
    TMatrix4x4<T>   ToMatrixNoScale()   const;
    TTransform      InverseFast()       const;

    // ── Point / direction transform ───────────────────────────────────────────

    constexpr TVector<T>    TransformPoint              (TVector<T> p) const;
    constexpr TVector<T>    TransformDirection          (TVector<T> d) const;
    constexpr TVector<T>    TransformDirectionNoScale   (TVector<T> d) const;
              TVector<T>    InverseTransformPoint       (TVector<T> p) const;
              TVector<T>    InverseTransformDirection   (TVector<T> d) const;

    // ── Operators ─────────────────────────────────────────────────────────────

    constexpr bool  operator    ==  (const TTransform& r)   const;
    constexpr bool  operator    !=  (const TTransform& r)   const;
};

// ─── Free functions ───────────────────────────────────────────────────────────

template<std::floating_point T> TTransform<T>   Combine     (const TTransform<T>& parent, const TTransform<T>& child);
template<std::floating_point T> TTransform<T>   Lerp        (const TTransform<T>& a, const TTransform<T>& b, T t);
template<std::floating_point T> TTransform<T>   SlerpLerp   (const TTransform<T>& a, const TTransform<T>& b, T t);

// ─── Type aliases ─────────────────────────────────────────────────────────────

using FTransform  = TTransform<double>;
using FTransformF = TTransform<float>;

} // namespace CE

#include <Core/Math/Transform.inl>