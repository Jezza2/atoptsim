#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include <utility>
#include <vector>

#include "real_type.h"
#include "vec3.h"

class ray;

struct hit_record
{
    position p;
    direction normal;
    real_t t;
    bool front_face;

    void set_face_normal(const ray& r, const direction& outward_normal);
};

class hittable
{
public:
    virtual ~hittable() = default;

    virtual bool hit(
            const ray& r,
            real_t t_min,
            real_t t_max,
            hit_record& rec) const = 0;
};

class hittable_list : public hittable
{
public:
    void clear() { _objs.clear(); }
    void add(std::unique_ptr<hittable> h) { _objs.push_back(std::move(h)); }

    bool hit(const ray& r, real_t t_min, real_t t_max, hit_record& rec) const final;

private:
    std::vector<std::unique_ptr<hittable>> _objs;
};

#endif