#ifndef CAMERA_H
#define CAMERA_H

#include "real_type.h"
#include "vec3.h"
#include "ray.h"

class camera
{
public:
    camera();

    ray get_ray(real_t u, real_t v) const;

private:
    position _origin;
    direction _horizontal;
    direction _vertical;
    position _lower_left_corner;
};

#endif