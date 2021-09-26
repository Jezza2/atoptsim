#include "image.h"

#include <utility>

image::image(std::size_t width, std::size_t height)
:   _width(width)
,   _height(height)
{
    _pixels.resize(height * width);
}

image::image(std::size_t width, real_t aspect_ratio)
:   image(width, static_cast<std::size_t>(width / aspect_ratio))
{
}

image::image(const image& img)
:   _width(img._width)
,   _height(img._height)
,   _pixels(img._pixels)
{
}

image& image::operator=(image img)
{
    swap(*this, img);
    return *this;
}

image::image(image&& img) noexcept
{
    swap(*this, img);
}

image& image::operator=(image&& img) noexcept
{
    swap(*this, img);
    return *this;
}

void swap(image& lhs, image& rhs) noexcept
{
    using std::swap;
    swap(lhs._height, rhs._height);
    swap(lhs._width, rhs._width);
    swap(lhs._pixels, rhs._pixels);
}

const colour& image::at(std::size_t x, std::size_t y) const
{
    return _pixels.at(y * _width + x);
}

colour& image::at(std::size_t x, std::size_t y)
{
    return _pixels.at(y * _width + x);
}

std::size_t image::height() const
{
    return _height;
}

std::size_t image::width() const
{
    return _width;
}

void image::scale_brightness(real_t sf)
{
    for (colour& pixel : _pixels)
        pixel *= sf;
}

namespace
{

real_t clamp(real_t x, real_t cmin, real_t cmax)
{
    if (x < cmin)
        return cmin;
    else if (x > cmax)
        return cmax;
    else
        return x;
}

std::ostream& operator<<(std::ostream& os, const colour& c)
{
    return os << static_cast<int>(256 * clamp(c.x, 0.0, 0.9999))
            << ' ' << static_cast<int>(256 * clamp(c.y, 0.0, 0.9999))
            << ' ' << static_cast<int>(256 * clamp(c.z, 0.0, 0.9999));
}

} /* Anonymous namespace */

std::ostream& operator<<(std::ostream& os, const image& img)
{
    os << "P3\n";
    os << img._width << ' ' << img._height << '\n';
    os << "255\n";

    for (const colour& pixel : img._pixels)
        os << pixel << '\n';

    return os;
}