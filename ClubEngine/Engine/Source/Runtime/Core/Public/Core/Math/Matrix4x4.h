// ─── AI-Assisted Code ────────────────────────────────────────────────────────
//
//  This file was generated with the assistance of Claude (Anthropic).
//  Reviewed and approved by: Shota Khakhishvili
//  Date: 2026-05-11
//
//  If you modify this file, remove or update this notice accordingly.
//
// ─────────────────────────────────────────────────────────────────────────────

#pragma once

#include <Core/Math/Vector.h>

#include <concepts>

namespace CE
{

template<std::floating_point T>
struct TMatrix4x4
{
    TVector4<T> cols[4];

    // -------------------------------------------------------------------------
    // Construction
    // -------------------------------------------------------------------------

    constexpr TMatrix4x4() = default;

    constexpr TMatrix4x4(
        TVector4<T> c0,
        TVector4<T> c1,
        TVector4<T> c2,
        TVector4<T> c3);

    constexpr TMatrix4x4(
        T c0r0, T c0r1, T c0r2, T c0r3,
        T c1r0, T c1r1, T c1r2, T c1r3,
        T c2r0, T c2r1, T c2r2, T c2r3,
        T c3r0, T c3r1, T c3r2, T c3r3);

    template<std::floating_point U>
    explicit constexpr TMatrix4x4(const TMatrix4x4<U>& o);

    // -------------------------------------------------------------------------
    // Element access
    // -------------------------------------------------------------------------

    constexpr       TVector4<T>& operator[](int col);
    constexpr const TVector4<T>& operator[](int col) const;

    /// Returns a reference to element [row][col]. Prefer Get/Set for const access.
    constexpr T& At(int row, int col);

    constexpr T    Get(int row, int col) const;
    constexpr void Set(int row, int col, T v);

    /// Pointer to the first element in column-major storage.
    const T* Data() const;

    // -------------------------------------------------------------------------
    // Static constructors
    // -------------------------------------------------------------------------

    static constexpr TMatrix4x4 Identity();
    static constexpr TMatrix4x4 Zero();

    static constexpr TMatrix4x4 Translation(T tx, T ty, T tz);
    static constexpr TMatrix4x4 Translation(TVector<T> t);

    static constexpr TMatrix4x4 Scale(T sx, T sy, T sz);
    static constexpr TMatrix4x4 Scale(T s);
    static constexpr TMatrix4x4 Scale(TVector<T> s);

    static TMatrix4x4 RotationX(T radians);
    static TMatrix4x4 RotationY(T radians);
    static TMatrix4x4 RotationZ(T radians);

    /// OpenGL-style perspective projection (right-handed, NDC depth [-1, 1]).
    /// @param fovY   Vertical field of view in radians.
    /// @param aspect Width / height ratio.
    /// @param zNear  Near clip plane (positive value).
    /// @param zFar   Far clip plane  (positive value).
    static TMatrix4x4 Perspective(T fovY, T aspect, T zNear, T zFar);

    /// Orthographic projection (right-handed, NDC depth [-1, 1]).
    static TMatrix4x4 Ortho(T left, T right, T bottom, T top, T zNear, T zFar);

    /// Standard view matrix — camera positioned at `eye`, looking at `center`.
    static TMatrix4x4 LookAt(TVector<T> eye, TVector<T> center, TVector<T> up);

    // -------------------------------------------------------------------------
    // Operators
    // -------------------------------------------------------------------------

    constexpr TMatrix4x4  operator* (const TMatrix4x4& r) const;
    constexpr TMatrix4x4& operator*=(const TMatrix4x4& r);

    constexpr TVector4<T> operator*(const TVector4<T>& v) const;

    constexpr bool operator==(const TMatrix4x4& r) const;
    constexpr bool operator!=(const TMatrix4x4& r) const;
};

// -----------------------------------------------------------------------------
// Free functions
// -----------------------------------------------------------------------------

template<std::floating_point T>
constexpr TMatrix4x4<T> Transpose(const TMatrix4x4<T>& m);

template<std::floating_point T>
T Determinant(const TMatrix4x4<T>& m);

template<std::floating_point T>
TMatrix4x4<T> Inverse(const TMatrix4x4<T>& m);

/// Transforms a point (w = 1 — translation applies).
template<std::floating_point T>
constexpr TVector<T> TransformPoint(const TMatrix4x4<T>& m, TVector<T> v);

/// Transforms a direction (w = 0 — translation does NOT apply).
template<std::floating_point T>
constexpr TVector<T> TransformDirection(const TMatrix4x4<T>& m, TVector<T> v);

// -----------------------------------------------------------------------------
// Type aliases
// -----------------------------------------------------------------------------

using FMatrix4x4  = TMatrix4x4<double>;
using FMatrix4x4F = TMatrix4x4<float>;

}

#include <Core/Math/Matrix4x4.inl>