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

#include <cmath>

namespace CE
{

// =============================================================================
// Construction
// =============================================================================

template<std::floating_point T>
constexpr TMatrix4x4<T>::TMatrix4x4(
    TVector4<T> c0, TVector4<T> c1,
    TVector4<T> c2, TVector4<T> c3)
    : cols{ c0, c1, c2, c3 }
{}

template<std::floating_point T>
constexpr TMatrix4x4<T>::TMatrix4x4(
    T c0r0, T c0r1, T c0r2, T c0r3,
    T c1r0, T c1r1, T c1r2, T c1r3,
    T c2r0, T c2r1, T c2r2, T c2r3,
    T c3r0, T c3r1, T c3r2, T c3r3)
    : cols{
        TVector4<T>{ c0r0, c0r1, c0r2, c0r3 },
        TVector4<T>{ c1r0, c1r1, c1r2, c1r3 },
        TVector4<T>{ c2r0, c2r1, c2r2, c2r3 },
        TVector4<T>{ c3r0, c3r1, c3r2, c3r3 }
      }
{}

template<std::floating_point T>
template<std::floating_point U>
constexpr TMatrix4x4<T>::TMatrix4x4(const TMatrix4x4<U>& o)
    : cols{
        TVector4<T>(o.cols[0]),
        TVector4<T>(o.cols[1]),
        TVector4<T>(o.cols[2]),
        TVector4<T>(o.cols[3])
      }
{}

// =============================================================================
// Element access
// =============================================================================

template<std::floating_point T>
constexpr TVector4<T>& TMatrix4x4<T>::operator[](int col)
{
    return cols[col];
}

template<std::floating_point T>
constexpr const TVector4<T>& TMatrix4x4<T>::operator[](int col) const
{
    return cols[col];
}

template<std::floating_point T>
constexpr T& TMatrix4x4<T>::At(int row, int col)
{
    return (&cols[col].x)[row];
}

template<std::floating_point T>
constexpr T TMatrix4x4<T>::Get(int row, int col) const
{
    const T* c = &cols[col].x;
    return c[row];
}

template<std::floating_point T>
constexpr void TMatrix4x4<T>::Set(int row, int col, T v)
{
    T* c = &cols[col].x;
    c[row] = v;
}

template<std::floating_point T>
const T* TMatrix4x4<T>::Data() const
{
    return &cols[0].x;
}

// =============================================================================
// Static constructors
// =============================================================================

template<std::floating_point T>
constexpr TMatrix4x4<T> TMatrix4x4<T>::Identity()
{
    return {
        { T(1), T(0), T(0), T(0) },
        { T(0), T(1), T(0), T(0) },
        { T(0), T(0), T(1), T(0) },
        { T(0), T(0), T(0), T(1) }
    };
}

template<std::floating_point T>
constexpr TMatrix4x4<T> TMatrix4x4<T>::Zero()
{
    return {
        { T(0), T(0), T(0), T(0) },
        { T(0), T(0), T(0), T(0) },
        { T(0), T(0), T(0), T(0) },
        { T(0), T(0), T(0), T(0) }
    };
}

template<std::floating_point T>
constexpr TMatrix4x4<T> TMatrix4x4<T>::Translation(T tx, T ty, T tz)
{
    return {
        { T(1), T(0), T(0), T(0) },
        { T(0), T(1), T(0), T(0) },
        { T(0), T(0), T(1), T(0) },
        { tx,   ty,   tz,   T(1) }
    };
}

template<std::floating_point T>
constexpr TMatrix4x4<T> TMatrix4x4<T>::Translation(TVector<T> t)
{
    return Translation(t.x, t.y, t.z);
}

template<std::floating_point T>
constexpr TMatrix4x4<T> TMatrix4x4<T>::Scale(T sx, T sy, T sz)
{
    return {
        { sx,   T(0), T(0), T(0) },
        { T(0), sy,   T(0), T(0) },
        { T(0), T(0), sz,   T(0) },
        { T(0), T(0), T(0), T(1) }
    };
}

template<std::floating_point T>
constexpr TMatrix4x4<T> TMatrix4x4<T>::Scale(T s)
{
    return Scale(s, s, s);
}

template<std::floating_point T>
constexpr TMatrix4x4<T> TMatrix4x4<T>::Scale(TVector<T> s)
{
    return Scale(s.x, s.y, s.z);
}

template<std::floating_point T>
TMatrix4x4<T> TMatrix4x4<T>::RotationX(T radians)
{
    const T c = std::cos(radians);
    const T s = std::sin(radians);
    return {
        { T(1), T(0), T(0), T(0) },
        { T(0),  c,    s,   T(0) },
        { T(0), -s,    c,   T(0) },
        { T(0), T(0), T(0), T(1) }
    };
}

template<std::floating_point T>
TMatrix4x4<T> TMatrix4x4<T>::RotationY(T radians)
{
    const T c = std::cos(radians);
    const T s = std::sin(radians);
    return {
        {  c,   T(0), -s,   T(0) },
        { T(0), T(1), T(0), T(0) },
        {  s,   T(0),  c,   T(0) },
        { T(0), T(0), T(0), T(1) }
    };
}

template<std::floating_point T>
TMatrix4x4<T> TMatrix4x4<T>::RotationZ(T radians)
{
    const T c = std::cos(radians);
    const T s = std::sin(radians);
    return {
        {  c,    s,   T(0), T(0) },
        { -s,    c,   T(0), T(0) },
        { T(0), T(0), T(1), T(0) },
        { T(0), T(0), T(0), T(1) }
    };
}

template<std::floating_point T>
TMatrix4x4<T> TMatrix4x4<T>::Perspective(T fovY, T aspect, T zNear, T zFar)
{
    const T f     = T(1) / std::tan(fovY * T(0.5));
    const T range = zNear - zFar;
    return {
        { f / aspect, T(0), T(0),                          T(0)  },
        { T(0),        f,   T(0),                          T(0)  },
        { T(0),       T(0), (zFar + zNear) / range,        T(-1) },
        { T(0),       T(0), (T(2) * zFar * zNear) / range, T(0)  }
    };
}

template<std::floating_point T>
TMatrix4x4<T> TMatrix4x4<T>::Ortho(T left, T right, T bottom, T top, T zNear, T zFar)
{
    const T rl = right - left;
    const T tb = top   - bottom;
    const T fn = zFar  - zNear;
    return {
        { T(2)/rl,           T(0),            T(0),             T(0) },
        { T(0),              T(2)/tb,          T(0),             T(0) },
        { T(0),              T(0),            T(-2)/fn,          T(0) },
        { -(right+left)/rl, -(top+bottom)/tb, -(zFar+zNear)/fn, T(1) }
    };
}

template<std::floating_point T>
TMatrix4x4<T> TMatrix4x4<T>::LookAt(TVector<T> eye, TVector<T> center, TVector<T> up)
{
    const TVector<T> f = Normalize(center - eye);
    const TVector<T> r = Normalize(Cross(f, up));
    const TVector<T> u = Cross(r, f);

    return {
        {  r.x,  u.x, -f.x, T(0) },
        {  r.y,  u.y, -f.y, T(0) },
        {  r.z,  u.z, -f.z, T(0) },
        { -Dot(r, eye), -Dot(u, eye), Dot(f, eye), T(1) }
    };
}

// =============================================================================
// Operators
// =============================================================================

template<std::floating_point T>
constexpr TMatrix4x4<T> TMatrix4x4<T>::operator*(const TMatrix4x4<T>& r) const
{
    TMatrix4x4<T> out;
    for (int col = 0; col < 4; ++col)
        for (int row = 0; row < 4; ++row)
        {
            T sum = T(0);
            for (int k = 0; k < 4; ++k)
                sum += Get(row, k) * r.Get(k, col);
            out.Set(row, col, sum);
        }
    return out;
}

template<std::floating_point T>
constexpr TMatrix4x4<T>& TMatrix4x4<T>::operator*=(const TMatrix4x4<T>& r)
{
    *this = *this * r;
    return *this;
}

template<std::floating_point T>
constexpr TVector4<T> TMatrix4x4<T>::operator*(const TVector4<T>& v) const
{
    return {
        Get(0,0)*v.x + Get(0,1)*v.y + Get(0,2)*v.z + Get(0,3)*v.w,
        Get(1,0)*v.x + Get(1,1)*v.y + Get(1,2)*v.z + Get(1,3)*v.w,
        Get(2,0)*v.x + Get(2,1)*v.y + Get(2,2)*v.z + Get(2,3)*v.w,
        Get(3,0)*v.x + Get(3,1)*v.y + Get(3,2)*v.z + Get(3,3)*v.w
    };
}

template<std::floating_point T>
constexpr bool TMatrix4x4<T>::operator==(const TMatrix4x4<T>& r) const
{
    return cols[0] == r.cols[0]
        && cols[1] == r.cols[1]
        && cols[2] == r.cols[2]
        && cols[3] == r.cols[3];
}

template<std::floating_point T>
constexpr bool TMatrix4x4<T>::operator!=(const TMatrix4x4<T>& r) const
{
    return !(*this == r);
}

// =============================================================================
// Free functions
// =============================================================================

template<std::floating_point T>
constexpr TMatrix4x4<T> Transpose(const TMatrix4x4<T>& m)
{
    TMatrix4x4<T> out;
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            out.Set(row, col, m.Get(col, row));
    return out;
}

template<std::floating_point T>
T Determinant(const TMatrix4x4<T>& m)
{
    const T* c = m.Data();

    auto sub3det = [](T a0, T a1, T a2,
                      T b0, T b1, T b2,
                      T d0, T d1, T d2) -> T
    {
        return a0*(b1*d2 - b2*d1)
             - a1*(b0*d2 - b2*d0)
             + a2*(b0*d1 - b1*d0);
    };

    return
        c[0]  * sub3det(c[5],c[9],c[13],  c[6],c[10],c[14],  c[7],c[11],c[15])
      - c[4]  * sub3det(c[1],c[9],c[13],  c[2],c[10],c[14],  c[3],c[11],c[15])
      + c[8]  * sub3det(c[1],c[5],c[13],  c[2],c[6], c[14],  c[3],c[7], c[15])
      - c[12] * sub3det(c[1],c[5],c[9],   c[2],c[6], c[10],  c[3],c[7], c[11]);
}

template<std::floating_point T>
TMatrix4x4<T> Inverse(const TMatrix4x4<T>& m)
{
    const T* s = m.Data();
    T inv[16];

    inv[0]  =  s[5]*s[10]*s[15] - s[5]*s[11]*s[14] - s[9]*s[6]*s[15]
             + s[9]*s[7]*s[14]  + s[13]*s[6]*s[11]  - s[13]*s[7]*s[10];
    inv[4]  = -s[4]*s[10]*s[15] + s[4]*s[11]*s[14]  + s[8]*s[6]*s[15]
             - s[8]*s[7]*s[14]  - s[12]*s[6]*s[11]  + s[12]*s[7]*s[10];
    inv[8]  =  s[4]*s[9]*s[15]  - s[4]*s[11]*s[13]  - s[8]*s[5]*s[15]
             + s[8]*s[7]*s[13]  + s[12]*s[5]*s[11]  - s[12]*s[7]*s[9];
    inv[12] = -s[4]*s[9]*s[14]  + s[4]*s[10]*s[13]  + s[8]*s[5]*s[14]
             - s[8]*s[6]*s[13]  - s[12]*s[5]*s[10]  + s[12]*s[6]*s[9];

    inv[1]  = -s[1]*s[10]*s[15] + s[1]*s[11]*s[14]  + s[9]*s[2]*s[15]
             - s[9]*s[3]*s[14]  - s[13]*s[2]*s[11]  + s[13]*s[3]*s[10];
    inv[5]  =  s[0]*s[10]*s[15] - s[0]*s[11]*s[14]  - s[8]*s[2]*s[15]
             + s[8]*s[3]*s[14]  + s[12]*s[2]*s[11]  - s[12]*s[3]*s[10];
    inv[9]  = -s[0]*s[9]*s[15]  + s[0]*s[11]*s[13]  + s[8]*s[1]*s[15]
             - s[8]*s[3]*s[13]  - s[12]*s[1]*s[11]  + s[12]*s[3]*s[9];
    inv[13] =  s[0]*s[9]*s[14]  - s[0]*s[10]*s[13]  - s[8]*s[1]*s[14]
             + s[8]*s[2]*s[13]  + s[12]*s[1]*s[10]  - s[12]*s[2]*s[9];

    inv[2]  =  s[1]*s[6]*s[15]  - s[1]*s[7]*s[14]   - s[5]*s[2]*s[15]
             + s[5]*s[3]*s[14]  + s[13]*s[2]*s[7]   - s[13]*s[3]*s[6];
    inv[6]  = -s[0]*s[6]*s[15]  + s[0]*s[7]*s[14]   + s[4]*s[2]*s[15]
             - s[4]*s[3]*s[14]  - s[12]*s[2]*s[7]   + s[12]*s[3]*s[6];
    inv[10] =  s[0]*s[5]*s[15]  - s[0]*s[7]*s[13]   - s[4]*s[1]*s[15]
             + s[4]*s[3]*s[13]  + s[12]*s[1]*s[7]   - s[12]*s[3]*s[5];
    inv[14] = -s[0]*s[5]*s[14]  + s[0]*s[6]*s[13]   + s[4]*s[1]*s[14]
             - s[4]*s[2]*s[13]  - s[12]*s[1]*s[6]   + s[12]*s[2]*s[5];

    inv[3]  = -s[1]*s[6]*s[11]  + s[1]*s[7]*s[10]   + s[5]*s[2]*s[11]
             - s[5]*s[3]*s[10]  - s[9]*s[2]*s[7]    + s[9]*s[3]*s[6];
    inv[7]  =  s[0]*s[6]*s[11]  - s[0]*s[7]*s[10]   - s[4]*s[2]*s[11]
             + s[4]*s[3]*s[10]  + s[8]*s[2]*s[7]    - s[8]*s[3]*s[6];
    inv[11] = -s[0]*s[5]*s[11]  + s[0]*s[7]*s[9]    + s[4]*s[1]*s[11]
             - s[4]*s[3]*s[9]   - s[8]*s[1]*s[7]    + s[8]*s[3]*s[5];
    inv[15] =  s[0]*s[5]*s[10]  - s[0]*s[6]*s[9]    - s[4]*s[1]*s[10]
             + s[4]*s[2]*s[9]   + s[8]*s[1]*s[6]    - s[8]*s[2]*s[5];

    const T det = s[0]*inv[0] + s[1]*inv[4] + s[2]*inv[8] + s[3]*inv[12];

    if (NearlyZero(det))
        return TMatrix4x4<T>::Identity();

    const T invDet = T(1) / det;

    TMatrix4x4<T> out;
    T* d = &out.cols[0].x;
    for (int i = 0; i < 16; ++i)
        d[i] = inv[i] * invDet;

    return out;
}

template<std::floating_point T>
constexpr TVector<T> TransformPoint(const TMatrix4x4<T>& m, TVector<T> v)
{
    return (m * TVector4<T>{ v.x, v.y, v.z, T(1) }).XYZ();
}

template<std::floating_point T>
constexpr TVector<T> TransformDirection(const TMatrix4x4<T>& m, TVector<T> v)
{
    return (m * TVector4<T>{ v.x, v.y, v.z, T(0) }).XYZ();
}

} // namespace CE