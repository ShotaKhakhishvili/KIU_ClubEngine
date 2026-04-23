#pragma once

struct Vec2f
{
    float x{0.0f};
    float y{0.0f};

    constexpr Vec2f() = default;
    constexpr Vec2f(float inX, float inY) : x(inX), y(inY) {}

    template<typename TVec2>
    explicit constexpr Vec2f(const TVec2& v)
        : x(static_cast<float>(v.x)), y(static_cast<float>(v.y))
    {}

    template<typename TVec2>
    constexpr Vec2f& operator=(const TVec2& v)
    {
        x = static_cast<float>(v.x);
        y = static_cast<float>(v.y);
        return *this;
    }
};

struct Vec3f
{
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};

    constexpr Vec3f() = default;
    constexpr Vec3f(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

    template<typename TVec3>
    explicit constexpr Vec3f(const TVec3& v)
        : x(static_cast<float>(v.x)), y(static_cast<float>(v.y)), z(static_cast<float>(v.z))
    {}

    template<typename TVec3>
    constexpr Vec3f& operator=(const TVec3& v)
    {
        x = static_cast<float>(v.x);
        y = static_cast<float>(v.y);
        z = static_cast<float>(v.z);
        return *this;
    }
};

struct Vec4f
{
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};
    float w{0.0f};

    constexpr Vec4f() = default;
    constexpr Vec4f(float inX, float inY, float inZ, float inW)
        : x(inX), y(inY), z(inZ), w(inW)
    {}

    template<typename TVec4>
    explicit constexpr Vec4f(const TVec4& v)
        : x(static_cast<float>(v.x)), y(static_cast<float>(v.y)), z(static_cast<float>(v.z)), w(static_cast<float>(v.w))
    {}

    template<typename TVec4>
    constexpr Vec4f& operator=(const TVec4& v)
    {
        x = static_cast<float>(v.x);
        y = static_cast<float>(v.y);
        z = static_cast<float>(v.z);
        w = static_cast<float>(v.w);
        return *this;
    }
};