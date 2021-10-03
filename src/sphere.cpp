#include "sphere.h"

#include <cmath>
#include <stdexcept>

sphere::sphere(const position& centre, real_t radius, const material* mat)
:   _centre(centre)
,   _radius(radius)
,   _material(mat)
{
    if (_material == nullptr)
        std::runtime_error("No material set for sphere");
}

bool sphere::hit(const ray& r, real_t t_min, real_t t_max, hit_record& rec) const
{
    position relpos = r.origin() - _centre;
    const real_t a = r.dir().length2();
    const real_t half_b = dot(relpos, r.dir());
    const real_t c = relpos.length2() - _radius*_radius;

    const real_t discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
        return false;

    const real_t sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    real_t root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    direction outward_normal = (rec.p - _centre) / _radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = _material;

    return true;
}