#include "vec3.h"
#include "image.h"
#include "ray.h"
#include "camera.h"
#include "hittable.h"
#include "sphere.h"
#include "rt_utils.h"

#include <iostream>

#ifndef NO_OPENMP
#include <omp.h>
#endif

colour ray_colour(const ray& r, const hittable& h)
{
    hit_record rec;
    if (h.hit(r, 0.0, infinity, rec))
        return 0.5 * (static_cast<colour>(rec.normal) + colour(1,1,1));

    const direction unit_dir = normalise(r.dir());
    const real_t t = 0.5 * (unit_dir.y + 1.0);
    return (1.0 - t) * colour(1, 1, 1) + t * colour(0.5, 0.7, 1.0);
}

std::string progress_bar(int pc_progress)
{
    constexpr int num_blocks = 50;
    constexpr int block_width_pm = 1000 / num_blocks;
    const int block_progress = 10 * pc_progress / block_width_pm;

    std::string bar = "[";

    for (int i = 0; i < block_progress; ++i)
        bar += '#';
    for (int i = block_progress; i < num_blocks; ++i)
        bar += ' ';
    
    bar += ']';

    return bar;
}

int main(int argc, char* argv[])
{
    const real_t aspect_ratio = 16.0 / 9.0;
    image rainbow(1920, aspect_ratio);

    hittable_list world;
    world.add(std::make_unique<sphere>(position(0.0, 0.0, -1.0), 0.5));
    world.add(std::make_unique<sphere>(position(0.0, -100.5, -1.0), 100));

    const camera cam;

    const size_t img_width = rainbow.width();
    const size_t img_height = rainbow.height();
    const int samples_per_pixel = 100;

    int prev_progress = 0;
    for (size_t j = 0; j < img_height; ++j)
    {
        const int pc_progress = j * 100 / (img_height - 1);
        if (pc_progress != prev_progress)
            std::cerr << "\rProgress: " << progress_bar(pc_progress) << ' ' << pc_progress << '%';
        prev_progress = pc_progress;

        #pragma omp parallel for
        for (size_t i = 0; i < img_width; ++i)
        {
            for (int k = 0; k < samples_per_pixel; ++k)
            {
                const real_t u = (i + random_real(-0.5, 0.5)) / (img_width-1);
                const real_t v = (j + random_real(-0.5, 0.5)) / (img_height-1);
                const ray r = cam.get_ray(u, v);
                rainbow.at(i, img_height - 1 - j) += ray_colour(r, world);
            }
        }
    }

    std::cerr << '\n';

    rainbow.scale_brightness(1.0 / samples_per_pixel);

    std::cout << rainbow;
}
