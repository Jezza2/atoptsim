#include "hittable.h"
#include "ray.h"

void hit_record::set_face_normal(const ray& r, const direction& outward_normal)
{
    front_face = dot(r.dir(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
}

bool hittable_list::hit(const ray& r, real_t t_min, real_t t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    real_t closest_so_far = t_max;

    for (const std::unique_ptr<hittable>& obj : _objs)
    {
        if (obj->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}