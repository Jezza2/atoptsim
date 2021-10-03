#ifndef RT_UTILS_H
#define RT_UTILS_H

#include <limits>
#include <random>

#include "real_type.h"
#include "vec3.h"

constexpr real_t infinity = std::numeric_limits<real_t>::infinity();
constexpr real_t pi = 3.1415926535897932385;

constexpr real_t to_radians(real_t degrees)
{
    return degrees * pi / 180.0;
}

inline real_t random_real()
{
    static std::uniform_real_distribution<real_t> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline real_t random_real(real_t rmin, real_t rmax)
{
    return rmin + (rmax - rmin) * random_real();
}

template <typename VecT>
VecT random_vector()
{
    return { random_real(), random_real(), random_real() };
}

template <typename VecT>
VecT random_vector(real_t vmin, real_t vmax)
{
    return { random_real(vmin, vmax), random_real(vmin, vmax), random_real(vmin, vmax) };
}

template <typename VecT>
VecT random_unit_vector()
{
    return random_vector<VecT>(-1.0, 1.0).normalise();
}

template <typename VecT>
VecT reflect(VecT v, const VecT& n)
{
    return v - 2 * dot(v, n) * n;
}

template <typename VecT>
VecT refract(VecT u, const VecT& n, real_t n_ratio) {
    const real_t cos_theta = std::fmin(dot(-u, n), 1.0);
    const VecT r_out_perp =  n_ratio * (u + cos_theta * n);
    const VecT r_out_parallel = -std::sqrt(fabs(1.0 - r_out_perp.length2())) * n;
    return r_out_perp + r_out_parallel;
}

inline real_t clamp(real_t x, real_t cmin, real_t cmax)
{
    if (x < cmin)
        return cmin;
    else if (x > cmax)
        return cmax;
    else
        return x;
}

#endif