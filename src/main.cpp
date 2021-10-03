#include "vec3.h"
#include "image.h"
#include "ray.h"
#include "camera.h"
#include "hittable.h"
#include "sphere.h"
#include "rt_utils.h"
#include "material.h"

#include <iostream>

#ifndef NO_OPENMP
#include <omp.h>
#endif

namespace materials
{

// Materials are just static

const lambertian ground(colour(0.8, 0.8, 0.0));
const lambertian centre(colour(0.7, 0.3, 0.3));
const metal left(colour(0.8, 0.8, 0.8), 0.0);
const metal right(colour(0.8, 0.6, 0.2), 1.0);
const dielectric glass(1.5);

}

colour ray_colour(const ray& r, const hittable& h, int depth)
{
    if (depth <= 0)
        return { 0.0, 0.0, 0.0 };

    hit_record rec;
    if (h.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        colour attenuation;
        if (rec.mat && rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_colour(scattered, h, depth - 1);
        return { 0.0, 0.0, 0.0 };
    }

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
    image rainbow(600, aspect_ratio);

    hittable_list world;
    world.add(std::make_unique<sphere>(position(0.0, -100.5, -1.0), 100, &materials::ground));
    world.add(std::make_unique<sphere>(position(0.0, 0.0, -1.0), 0.5, &materials::centre));
    world.add(std::make_unique<sphere>(position(-1.0, 0.0, -1.0), 0.5, &materials::left));
    world.add(std::make_unique<sphere>(position( 1.0, 0.0, -1.0), 0.5, &materials::glass));
    world.add(std::make_unique<sphere>(position( 1.0, 0.0, -1.0), -0.45, &materials::glass));
    const camera cam;

    const size_t img_width = rainbow.width();
    const size_t img_height = rainbow.height();
    const int samples_per_pixel = 500;
    const int max_depth = 100;

    int prev_progress = -1;
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
                rainbow.at(i, img_height - 1 - j) += ray_colour(r, world, max_depth);
            }
        }
    }

    std::cerr << '\n';

    // Average out samples
    rainbow.scale_brightness(1.0 / samples_per_pixel);

    // Gamma correction
    rainbow.transform([] (real_t m) { return std::sqrt(m); });

    std::cout << rainbow;
}
