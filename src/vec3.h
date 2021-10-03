#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <utility>

#include "real_type.h"

enum class vec_type
{
    physical,
    colour
};

/**
 * 3D vector class, optimised for POD types
 */
template <vec_type N, typename T = double>
class vec3
{
    template <typename V> using vec_t = vec3<N, V>;

public:
    T x, y, z;

    constexpr vec3() = default;

    constexpr vec3(T x_, T y_, T z_)
    :   x(x_)
    ,   y(y_)
    ,   z(z_)
    {}
    
    template <typename U>
    constexpr vec3(const vec_t<U>& v)
    :   x(v.x)
    ,   y(v.y)
    ,   z(v.z)
    {}
    
    template <typename U>
    constexpr vec3(vec_t<T>&& v) noexcept
    :   x(std::move(v.x))
    ,   y(std::move(v.y))
    ,   z(std::move(v.z))
    {}

    template <typename U>
    constexpr vec_t<T>& operator=(vec_t<U> v)
    {
        swap(*this, v);
        return *this;
    }
    
    template <typename U>
    constexpr vec_t<T>& operator=(vec_t<U>&& v) noexcept
    {
        swap(*this, v);
        return *this;
    }

    template <typename U>
    constexpr vec_t<T>& operator+=(const vec_t<U>& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    template <typename U>
    constexpr vec_t<T>& operator-=(const vec_t<U>& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename U>
    constexpr vec_t<T>& operator*=(U s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    template <typename U>
    constexpr vec_t<T>& operator/=(U s)
    {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    template <typename U>
    constexpr vec_t<T>& cross(const vec_t<U>& v)
    {
        x = y*v.z - z*v.y;
        y = z*v.x - x*v.z;
        z = x*v.y - y*v.x;
        return *this;
    }

    constexpr T length2() const
    {
        return x*x + y*y + z*z;
    }

    constexpr T length() const
    {
        return std::sqrt(length2());
    }

    constexpr vec_t<T>& normalise()
    {
        return *this /= length();
    }

    template <typename FuncT>
    constexpr vec_t<T> transform(FuncT&& func)
    {
        return { func(x), func(y), func(z) };
    }

    constexpr bool near_zero() const
    {
        constexpr real_t s = 1e-8;
        return std::fabs(x) < s
                && std::fabs(y) < s
                && std::fabs(z) < s;
    }

    // Allow explicit casts to other types
    template <vec_type M>
    explicit operator vec3<M, T>() const
    {
        return { x, y, z };
    }
};

template <vec_type N, typename T, typename U>
constexpr void swap(vec3<N, T>& lhs, vec3<N, U>& rhs) noexcept
{
    using std::swap;
    swap(lhs.x, rhs.x);
    swap(lhs.y, rhs.y);
    swap(lhs.z, rhs.z);
}

template <vec_type N, typename T, typename U>
constexpr vec3<N, T> operator+(vec3<N, T> lhs, const vec3<N, U>& rhs)
{
    return lhs += rhs;
}

template <vec_type N, typename T, typename U>
constexpr vec3<N, T> operator-(vec3<N, T> lhs, const vec3<N, U>& rhs)
{
    return lhs -= rhs;
}

template <vec_type N, typename T, typename U>
constexpr vec3<N, T> operator*(vec3<N, T> lhs, const vec3<N, U>& rhs)
{
    return { lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z };
}

template <vec_type N, typename T, typename U>
constexpr vec3<N, T> operator*(vec3<N, T> v, U s)
{
    return v *= s;
}

template <vec_type N, typename T, typename U>
constexpr vec3<N, T> operator*(U s, vec3<N, T> v)
{
    return v * s;
}

template <vec_type N, typename T>
constexpr vec3<N, T> operator+(vec3<N, T> v)
{
    return v;
}

template <vec_type N, typename T>
constexpr vec3<N, T> operator-(vec3<N, T> v)
{
    return v * -1;
}

template <vec_type N, typename T, typename U>
constexpr vec3<N, T> operator/(vec3<N, T> v, U s)
{
    return v /= s;
}

template <vec_type N, typename T, typename U>
constexpr vec3<N, T> operator/(U s, vec3<N, T> v)
{
    return v / s;
}

template <vec_type N, typename T, typename U>
constexpr T dot(const vec3<N, T>& lhs, const vec3<N, U>& rhs)
{
    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}

template <vec_type N, typename T, typename U>
constexpr vec3<N, T> cross(vec3<N, T> lhs, const vec3<N, U>& rhs)
{
    return lhs.cross(rhs);
}

template <vec_type N, typename T>
constexpr vec3<N, T> normalise(vec3<N, T> v)
{
    return v.normalise();
}

template <vec_type N, typename T, typename U>
constexpr bool operator==(const vec3<N, T>& lhs, const vec3<N, U>& rhs)
{
    return lhs.x == rhs.x
            && lhs.y == rhs.y
            && lhs.z == rhs.z;
}

template <vec_type N, typename T, typename U>
constexpr bool operator!=(const vec3<N, T>& lhs, const vec3<N, U>& rhs)
{
    return !(lhs == rhs);
}

using number_type = double;

using position = vec3<vec_type::physical, real_t>;
using direction = vec3<vec_type::physical, real_t>;
using colour = vec3<vec_type::colour, real_t>;

#endif