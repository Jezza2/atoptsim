#include "material.h"

#include "vec3.h"
#include "ray.h"
#include "rt_utils.h"
#include "hittable.h"

bool lambertian::scatter(
        const ray& ray_in,
        const hit_record& rec,
        colour& attenuation,
        ray& scattered) const
{
    direction scatter_dir = rec.normal + random_unit_vector<direction>();

    // Catch degenerate scatter direction
    if (scatter_dir.near_zero())
        scatter_dir = rec.normal;

    scattered = ray(rec.p, scatter_dir);
    attenuation = _albedo;
    return true;
}


bool metal::scatter(
        const ray& ray_in,
        const hit_record& rec,
        colour& attenuation,
        ray& scattered) const
{
    const direction reflected = reflect(normalise(ray_in.dir()), rec.normal);
    scattered = ray(rec.p, reflected + _fuzz * random_unit_vector<direction>());
    attenuation = _albedo;
    return dot(scattered.dir(), rec.normal) > 0;
}

bool dielectric::scatter(
        const ray& r_in,
        const hit_record& rec,
        colour& attenuation,
        ray& scattered) const
{
    attenuation = colour(1.0, 1.0, 1.0);
    const real_t refraction_ratio = rec.front_face ? (1.0/_ir) : _ir;

    const direction unit_direction = normalise(r_in.dir());
    const real_t cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    const real_t sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

    const bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    // Total internal reflection or refraction    
    direction dir;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_real())
        dir = reflect(unit_direction, rec.normal);
    else
        dir = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = ray(rec.p, dir);
    return true;
}

real_t dielectric::reflectance(real_t cosine, real_t ref_idx)
{
    // Use Schlick's approximation for reflectance.
    const real_t r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    const real_t r2 = r0*r0;
    return r2 + (1.0 - r2) * std::pow((1 - cosine), 5);
}

bool light::scatter(
        const ray& r_in,
        const hit_record& rec,
        colour& attenuation,
        ray& scattered) const
{
    return false;
}
