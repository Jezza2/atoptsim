#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

struct hit_record;
class ray;

class material
{
public:
    virtual bool scatter(
            const ray& r_in,
            const hit_record& rec,
            colour& attenuation,
            ray& scattered) const = 0;
};

class lambertian : public material
{
public:
    explicit lambertian(const colour& a) : _albedo(a) {}

    bool scatter(
            const ray& ray_in,
            const hit_record& rec,
            colour& attenuation,
            ray& scattered) const override;

private:
    colour _albedo;
};

class metal : public material
{
public:
    metal(const colour& a, real_t f)
    :   _albedo(a)
    ,   _fuzz(f < 1 ? f : 1)
    {}

    bool scatter(
            const ray& ray_in,
            const hit_record& rec,
            colour& attenuation,
            ray& scattered) const override;

private:
    colour _albedo;
    real_t _fuzz;
};

class dielectric : public material
{
public:
    explicit dielectric(real_t ior) : _ir(ior) {}

    bool scatter(
            const ray& r_in,
            const hit_record& rec,
            colour& attenuation,
            ray& scattered) const override;

private:
    static real_t reflectance(real_t cosine, real_t ref_idx);

    real_t _ir; // Index of Refraction
};

#endif