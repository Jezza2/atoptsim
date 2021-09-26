#include "camera.h"

constexpr real_t aspect_ratio = 16.0 / 9.0;
constexpr real_t vp_height = 2.0;
constexpr real_t vp_width = aspect_ratio * vp_height;
constexpr direction focal_vec(0.0, 0.0, -1.0);

camera::camera()
:   _origin(0.0, 0.0, 0.0)
,   _horizontal(vp_width, 0.0, 0.0)
,   _vertical(0.0, vp_height, 0.0)
,   _lower_left_corner(_origin - _horizontal/2 - _vertical/2 + focal_vec)
{
}

ray camera::get_ray(real_t u, real_t v) const
{
    const direction dir =
            _lower_left_corner
            + u*_horizontal
            + v*_vertical
            - _origin;

    return ray(_origin, dir);
}