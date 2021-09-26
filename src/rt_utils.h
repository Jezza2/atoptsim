#ifndef RT_UTILS_H
#define RT_UTILS_H

#include <limits>
#include <random>

#include "real_type.h"

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