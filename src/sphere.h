#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include "real_type.h"

class ray;

class sphere : public hittable
{
public:
    sphere(const position& centre, real_t radius);

    bool hit(const ray& r, real_t t_min, real_t t_max, hit_record& rec) const final;

private:
    position _centre;
    real_t _radius;
};

#endif