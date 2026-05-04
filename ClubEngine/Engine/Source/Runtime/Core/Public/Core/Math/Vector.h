#pragma once

#include <Core/Math/MathCore.h>

#include <concepts>
#include <cmath>

namespace CE
{

template<typename T>
struct TVector2
{
    T x{}, y{};

    constexpr TVector2() = default;
    constexpr TVector2(T x, T y) : x(x), y(y) {}

    template<typename U>
    explicit constexpr TVector2(const TVector2<U>& other)
        :   x(static_cast<T>(other.x))
        ,   y(static_cast<T>(other.y))
    {}

    constexpr TVector2  operator    +   (const TVector2& r)     const   { return {x + r.x, y + r.y};        }
    constexpr TVector2  operator    -   (const TVector2& r)     const   { return {x-r.x, y-r.y};            }
    constexpr TVector2  operator    -   ()                      const   { return {-x, -y};                  }
    constexpr TVector2  operator    *   (const TVector2& r)     const   { return {x*r.x, y*r.y};            }
    constexpr TVector2  operator    *   (T s)                   const   { return {x*s,   y*s};              }

    constexpr bool      operator    ==  (const TVector2& r)     const   { return x==r.x && y==r.y;          }
    constexpr bool      operator    !=  (const TVector2& r)     const   { return !(*this == r);             }

    constexpr TVector2& operator    +=  (const TVector2& r)             { x+=r.x; y+=r.y; return *this;     }
    constexpr TVector2& operator    -=  (const TVector2& r)             { x-=r.x; y-=r.y; return *this;     }
    constexpr TVector2& operator    *=  (const TVector2& r)             { x*=r.x; y*=r.y; return *this;     } 
    constexpr TVector2& operator    *=  (T s)                           { x*=s;   y*=s;   return *this;     }

    constexpr TVector2 operator /(T r) const
        requires std::floating_point<T>
    {
        return {x / r, y / r};
    }

    constexpr TVector2& operator /=(T r)
        requires std::floating_point<T>
    {
        x /= r; y /= r;
        return *this;
    }
};

template<typename T>
struct TVector
{
    T x{}, y{}, z{};

    constexpr TVector() = default;
    constexpr TVector(T x, T y, T z) : x(x), y(y), z(z) {}

    template<typename U>
    explicit constexpr TVector(const TVector<U>& other)
        :   x(static_cast<T>(other.x))
        ,   y(static_cast<T>(other.y))
        ,   z(static_cast<T>(other.z))
    {}

    constexpr TVector   operator    +   (const TVector& r)      const   { return {x + r.x, y + r.y, z + r.z};   }
    constexpr TVector   operator    -   (const TVector& r)      const   { return {x-r.x, y-r.y, z-r.z};         }
    constexpr TVector   operator    -   ()                      const   { return {-x, -y, -z};                  }
    constexpr TVector   operator    *   (T s)                   const   { return {x*s,   y*s,   z*s};           }
    constexpr TVector   operator    *   (const TVector& r)      const   { return {x*r.x, y*r.y, z*r.z};         }

    constexpr bool      operator    ==  (const TVector& r)      const   { return x==r.x && y==r.y && z==r.z;    }
    constexpr bool      operator    !=  (const TVector& r)      const   { return !(*this == r);                 }

    constexpr TVector&  operator    +=  (const TVector& r)              { x+=r.x; y+=r.y; z+=r.z; return *this; }
    constexpr TVector&  operator    -=  (const TVector& r)              { x-=r.x; y-=r.y; z-=r.z; return *this; }
    constexpr TVector&  operator    *=  (const TVector& r)              { x*=r.x; y*=r.y; z*=r.z; return *this; }     
    constexpr TVector&  operator    *=  (T s)                           { x*=s;   y*=s;   z*=s;   return *this; }

    constexpr TVector operator /(T r) const
        requires std::floating_point<T>
    {
        return {x / r, y / r, z / r};
    }

    constexpr TVector& operator /=(T r) 
        requires std::floating_point<T>
    {
        x /= r; y /= r; z /= r;
        return *this;
    }

    constexpr TVector2<T> XY() const { return {x, y}; }
};

template<typename T>
struct TVector4
{
    T x{}, y{}, z{}, w{};

    constexpr TVector4() = default;
    constexpr TVector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    constexpr TVector4(TVector<T> v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

    template<typename U>
    explicit constexpr TVector4(const TVector4<U>& other)
        :   x(static_cast<T>(other.x))
        ,   y(static_cast<T>(other.y))
        ,   z(static_cast<T>(other.z))
        ,   w(static_cast<T>(other.w))
    {}

    constexpr TVector4  operator    +   (const TVector4& r)     const   { return {x+r.x, y+r.y, z+r.z, w+r.w};              }
    constexpr TVector4  operator    -   (const TVector4& r)     const   { return {x-r.x, y-r.y, z-r.z, w-r.w};              }
    constexpr TVector4  operator    -   ()                      const   { return {-x, -y, -z, -w};                          }
    constexpr TVector4  operator    *   (T s)                   const   { return {x*s,   y*s,   z*s, w*s};                  }
    constexpr TVector4  operator    *   (const TVector4& r)     const   { return {x*r.x, y*r.y, z*r.z, w*r.w};              }

    constexpr bool      operator    ==  (const TVector4& r)     const   { return x==r.x && y==r.y && z==r.z && w==r.w;      }
    constexpr bool      operator    !=  (const TVector4& r)     const   { return !(*this == r);                             }

    constexpr TVector4& operator    +=  (const TVector4& r)             { x+=r.x; y+=r.y; z+=r.z; w += r.w; return *this;   }
    constexpr TVector4& operator    -=  (const TVector4& r)             { x-=r.x; y-=r.y; z-=r.z; w -= r.w; return *this;   }
    constexpr TVector4& operator    *=  (const TVector4& r)             { x*=r.x; y*=r.y; z*=r.z; w*=r.w;   return *this;   } 
    constexpr TVector4& operator    *=  (T s)                           { x*=s;   y*=s;   z*=s;   w*=s;     return *this;   }

    constexpr TVector4 operator /(T r) const
        requires std::floating_point<T>
    {
        return {x / r, y / r, z / r, w / r};
    }

    constexpr TVector4& operator /=(T r)
        requires std::floating_point<T>
    {
        x /= r; y /= r; z /= r; w /= r;
        return *this;
    }

    constexpr TVector<T> XYZ() const { return {x, y, z}; }
    constexpr TVector2<T> XY() const { return {x, y}; }
};

template<typename T>
constexpr TVector2<T>   operator    *   (T s, const TVector2<T>& v)  { return v * s; }
template<typename T>
constexpr TVector<T>    operator    *   (T s, const TVector<T>& v)   { return v * s; }
template<typename T>
constexpr TVector4<T>   operator    *   (T s, const TVector4<T>& v)  { return v * s; }

using FVector = TVector<double>;
using FVectorF = TVector<float>;
using FVectorI = TVector<int>;

using FVector2F = TVector2<float>;
using FVector2D = TVector2<double>;
using FVector2I = TVector2<int>;

using FVector4F = TVector4<float>;
using FVector4D = TVector4<double>;
using FVector4I = TVector4<int>;

template<typename T>
constexpr T Dot (TVector2<T> a, TVector2<T> b)   { return a.x*b.x + a.y*b.y;                     }
template<typename T>
constexpr T Dot (TVector<T> a, TVector<T> b)     { return a.x*b.x + a.y*b.y + a.z*b.z;           }
template<typename T>
constexpr T Dot (TVector4<T> a, TVector4<T> b)   { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

template<typename T>
constexpr TVector<T> Cross(TVector<T> a, TVector<T> b)
{
    return {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

template<typename T>
constexpr T LengthSq    (TVector2<T> v) { return Dot(v, v); }
template<typename T>
constexpr T LengthSq    (TVector<T> v)  { return Dot(v, v); }
template<typename T>
constexpr T LengthSq    (TVector4<T> v) { return Dot(v, v); }

template<typename T>
inline T Length   (TVector2<T> v)                           { return std::sqrt(Dot(v, v));  }
template<typename T>
inline T Length   (TVector<T> v)                            { return std::sqrt(Dot(v, v));  }
template<typename T>
inline T Length   (TVector4<T> v)                           { return std::sqrt(Dot(v, v));  }

template<typename T>
constexpr TVector2<T>   Normalize(TVector2<T> v)            { return v / Length(v);         }
template<typename T>
constexpr TVector<T>    Normalize(TVector<T> v)             { return v / Length(v);         }
template<typename T>
constexpr TVector4<T>   Normalize(TVector4<T> v)            { return v / Length(v);         }

template<typename T>
constexpr bool NearlyEqual(TVector2<T> a, TVector2<T> b, T tolerance)
{
    return std::abs(a.x - b.x) < tolerance
        && std::abs(a.y - b.y) < tolerance;
}

template<typename T>
constexpr bool NearlyEqual(TVector<T> a, TVector<T> b, T tolerance)
{
    return std::abs(a.x-b.x) < tolerance
        && std::abs(a.y-b.y) < tolerance
        && std::abs(a.z-b.z) < tolerance;
}
 
template<typename T>
constexpr bool NearlyEqual(TVector4<T> a, TVector4<T> b, T tolerance)
{
    return std::abs(a.x-b.x) < tolerance
        && std::abs(a.y-b.y) < tolerance
        && std::abs(a.z-b.z) < tolerance
        && std::abs(a.w-b.w) < tolerance;
}

}