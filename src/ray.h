#ifndef RAY_H
#define RAY_H

#include "real_type.h"
#include "vec3.h"

class ray
{
public:
    ray(const position& origin, const direction& dir)
    :   _origin(origin)
    ,   _dir(dir)
    {}

    const position& origin() const { return _origin; }
    const direction& dir() const { return _dir; }

    position at(real_t t) const
    {
        return _origin + t*_dir;
    }

private:
    position _origin;
    direction _dir;
};

#endif