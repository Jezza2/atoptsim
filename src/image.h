#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <ostream>

#include "vec3.h"
#include "real_type.h"

class image
{
public:
    image(std::size_t width, std::size_t height);
    image(std::size_t width, real_t aspect_ratio);

    image(const image& img);
    image& operator=(image img);
    
    image(image&& img) noexcept;
    image& operator=(image&& img) noexcept;

    friend void swap(image& lhs, image& rhs) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const image& img);

    const colour& at(std::size_t x, std::size_t y) const;
    colour& at(std::size_t x, std::size_t y);

    std::size_t width() const;
    std::size_t height() const;

    void scale_brightness(real_t sf);

private:
    image() = default; // For copy and swap

    std::size_t _width;
    std::size_t _height;

    std::vector<colour> _pixels;
};

#endif