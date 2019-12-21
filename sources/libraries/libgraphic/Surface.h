#pragma once

#include <libmath/Math.h>
#include <libruntime/RefCounted.h>
#include <libruntime/Types.h>
#include <libsystem/Stream.h>

#include <libgraphic/Color.h>

namespace libgraphic
{

enum class SurfaceFiltering
{
    NEAREST,
    LINEAR,
};

class Surface : public libruntime::RefCounted<Surface>
{
private:
    int _width;
    int _height;
    Color *_buffer;

public:
    int width() { return _width; }
    int height() { return _height; }
    Size size() { return Size(_width, _height); }
    Bound bound() { return Bound(Point::zero(), size()); }

    Surface(int width, int height)
    {
        _buffer = new Color[width * height];
    }

    ~Surface()
    {
        delete _buffer;
    }

    Color sample(float x, float y, SurfaceFiltering filtering = SurfaceFiltering::NEAREST)
    {
        return sample(x, y, bound(), filtering);
    }

    Color sample(float x, float y, Bound source_rect, SurfaceFiltering filtering = SurfaceFiltering::NEAREST)
    {
        float xx = source_rect.width() * x;
        float yy = source_rect.height() * y;

        int xxi = (int)xx;
        int yyi = (int)yy;

        if (filtering == SurfaceFiltering::NEAREST)
        {
            return get_pixel(Point(source_rect.X() + xxi, source_rect.Y() + yyi));
        }
        else
        {
            Color c00 = get_pixel(Point(source_rect.X() + xxi, source_rect.Y() + yyi));
            Color c01 = get_pixel(Point(source_rect.X() + xxi, source_rect.Y() + yyi + 1));
            Color c10 = get_pixel(Point(source_rect.X() + xxi + 1, source_rect.Y() + yyi));
            Color c11 = get_pixel(Point(source_rect.X() + xxi + 1, source_rect.Y() + yyi + 1));

            return Color::lerp(
                Color::lerp(c00, c10, xx - xxi),
                Color::lerp(c01, c11, xx - xxi),

                yy - yyi);
        }
    }

    Color get_pixel(Point p)
    {
        int xi = libmath::abs((int)p.X() % _width);
        int yi = libmath::abs((int)p.Y() % _height);

        return _buffer[xi + yi * _width];
    }

    void set_pixel(Point p, Color color)
    {
        if ((p.X() >= 0 && p.X() < _width) && (p.Y() >= 0 && p.Y() < _height))
            _buffer[(int)(p.X() + p.Y() * _width)] = color;
    }
};

} // namespace libgraphic