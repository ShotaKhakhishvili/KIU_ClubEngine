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

namespace CE
{

// ─── Static factories ─────────────────────────────────────────────────────────

template<std::floating_point T>
constexpr TTransform<T> TTransform<T>::Identity()
{
    return {};
}

// ─── Conversion ───────────────────────────────────────────────────────────────

template<std::floating_point T>
TMatrix4x4<T> TTransform<T>::ToMatrix() const
{
    const TMatrix4x4<T> T_ = TMatrix4x4<T>::Translation(Translation);
    const TMatrix4x4<T> R_ = Rotation.ToMatrix();
    const TMatrix4x4<T> S_ = TMatrix4x4<T>::Scale(Scale);
    return T_ * R_ * S_;
}

template<std::floating_point T>
TMatrix4x4<T> TTransform<T>::ToMatrixNoScale() const
{
    const TMatrix4x4<T> T_ = TMatrix4x4<T>::Translation(Translation);
    const TMatrix4x4<T> R_ = Rotation.ToMatrix();
    return T_ * R_;
}

template<std::floating_point T>
TTransform<T> TTransform<T>::InverseFast() const
{
    const TVector<T> invScale {
        NearlyZero(Scale.x) ? T(0) : T(1) / Scale.x,
        NearlyZero(Scale.y) ? T(0) : T(1) / Scale.y,
        NearlyZero(Scale.z) ? T(0) : T(1) / Scale.z
    };
    const TQuat<T>   invRot   = Conjugate(Rotation);
    const TVector<T> invTrans = invRot * (-Translation * invScale);

    return { invTrans, invRot, invScale };
}

// ─── Point / direction transform ─────────────────────────────────────────────

template<std::floating_point T>
constexpr TVector<T> TTransform<T>::TransformPoint(TVector<T> p) const
{
    return Rotation * (Scale * p) + Translation;
}

template<std::floating_point T>
constexpr TVector<T> TTransform<T>::TransformDirection(TVector<T> d) const
{
    return Rotation * (Scale * d);
}

template<std::floating_point T>
constexpr TVector<T> TTransform<T>::TransformDirectionNoScale(TVector<T> d) const
{
    return Rotation * d;
}

template<std::floating_point T>
TVector<T> TTransform<T>::InverseTransformPoint(TVector<T> p) const
{
    return InverseFast().TransformPoint(p);
}

template<std::floating_point T>
TVector<T> TTransform<T>::InverseTransformDirection(TVector<T> d) const
{
    return InverseFast().TransformDirection(d);
}

// ─── Operators ────────────────────────────────────────────────────────────────

template<std::floating_point T>
constexpr bool TTransform<T>::operator==(const TTransform<T>& r) const
{
    return NearlyEqual(Translation, r.Translation, SMALL_NUMBER<T>)
        && NearlyEqual(Rotation,    r.Rotation,    SMALL_NUMBER<T>)
        && NearlyEqual(Scale,       r.Scale,       SMALL_NUMBER<T>);
}

template<std::floating_point T>
constexpr bool TTransform<T>::operator!=(const TTransform<T>& r) const { return !(*this == r); }

// ─── Free functions ───────────────────────────────────────────────────────────

template<std::floating_point T>
TTransform<T> Combine(const TTransform<T>& parent, const TTransform<T>& child)
{
    TTransform<T> out;
    out.Scale       = parent.Scale * child.Scale;
    out.Rotation    = parent.Rotation * child.Rotation;
    out.Translation = parent.TransformPoint(child.Translation);
    return out;
}

template<std::floating_point T>
TTransform<T> Lerp(const TTransform<T>& a, const TTransform<T>& b, T t)
{
    TTransform<T> out;
    out.Translation = a.Translation + (b.Translation - a.Translation) * t;
    out.Rotation    = Nlerp(a.Rotation, b.Rotation, t);
    out.Scale       = a.Scale + (b.Scale - a.Scale) * t;
    return out;
}

template<std::floating_point T>
TTransform<T> SlerpLerp(const TTransform<T>& a, const TTransform<T>& b, T t)
{
    TTransform<T> out;
    out.Translation = a.Translation + (b.Translation - a.Translation) * t;
    out.Rotation    = Slerp(a.Rotation, b.Rotation, t);
    out.Scale       = a.Scale + (b.Scale - a.Scale) * t;
    return out;
}

} // namespace CE