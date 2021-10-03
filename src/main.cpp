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
const dielectric water(1.33);
const light sun(colour(0.0, 1.0, 1.0));

}

colour ray_colour(const ray& r, const hittable& h, int depth)
{
    constexpr colour black(0.0, 0.0, 0.0);

    if (depth <= 0)
        return black;

    hit_record rec;
    if (h.hit(r, 0.001, infinity, rec))
    {
        if (rec.mat)
        {
            const colour emitted = rec.mat->emitted();
            
            ray scattered;
            colour attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return emitted + attenuation * ray_colour(scattered, h, depth - 1);
            
            return emitted;
        }

        return black;
    }

    return black;
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
    const real_t width = 600.0;
    const real_t height = width / aspect_ratio;
    image rainbow(static_cast<std::size_t>(width), aspect_ratio);

    hittable_list world;
    world.add(std::make_unique<sphere>(position(0.0, 0.0, 10.0), 1, &materials::sun));

    for (int i = 0; i < static_cast<int>(width); i += 10)
    {
        for (int j = 0; j < static_cast<int>(height); j += 10)
        {
            real_t x = -1.0*aspect_ratio + (2.0*aspect_ratio * (static_cast<real_t>(i)/width));
            real_t y = -1.0 + (2.0 * (static_cast<real_t>(j)/height));
            world.add(std::make_unique<sphere>(position(x, y, -1.0), 0.1, &materials::water));
        }
    }

    const camera cam;

    const size_t img_width = rainbow.width();
    const size_t img_height = rainbow.height();
    const int samples_per_pixel = 100;
    const int max_depth = 10;

    int prev_progress = -1;
    for (size_t j = 0; j < img_height; ++j)
    {
        const int pc_progress = j * 100 / (img_height - 1);
        if (pc_progress != prev_progress)
            std::cerr << "\rProgress: " << progress_bar(pc_progress) << ' ' << pc_progress << '%';
        prev_progress = pc_progress;

        // #pragma omp parallel for
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
