#pragma once

#include <libmath/MinMax.h>

#include <libgraphic/Point.h>
#include <libgraphic/Size.h>

namespace libgraphic
{

class Bound
{
private:
    int _x;
    int _y;
    int _width;
    int _height;

public:
    int X() { return _x; }
    int Y() { return _y; }
    int width() { return _width; }
    int height() { return _height; }

    Point location() { return Point(_x, _y); }
    Size size() { return Size(_width, _height); }

    int left() { return _x; }
    int right() { return _x + _width; }
    int top() { return _y; }
    int bottom() { return _y + _height; }

    Point topleft() { return Point(_x, _y); }
    Point topright() { return Point(_x + _width, _y); }
    Point bottomleft() { return Point(_x, _y + _height); }
    Point bottomright() { return Point(_x + _width, _y + _height); }

    Bound() : _x(0),
              _y(0),
              _width(0),
              _height(0)
    {
    }

    Bound(int width, int height) : _x(0),
                                   _y(0),
                                   _width(width),
                                   _height(height)
    {
    }

    Bound(int x, int y, int width, int height) : _x(x),
                                                 _y(y),
                                                 _width(width),
                                                 _height(height)
    {
    }

    Bound(Point position, Size size) : _x(position.X()),
                                       _y(position.Y()),
                                       _width(size.width()),
                                       _height(size.height())
    {
    }

    ~Bound() {}

    bool containe(Point point)
    {
        return point.X() >= left() &&
               point.X() < right() &&
               point.Y() >= top() &&
               point.Y() < bottom();
    }

    bool containe(Bound bound)
    {
        return left() < bound.right() &&
               right() > bound.left() &&
               top() < bound.bottom() &&
               bottom() > bound.top();
    }

    Bound clipped_in(Bound other)
    {
        auto x = libmath::max(other.X() + X(), other.X());
        auto y = libmath::max(other.Y() + Y(), other.Y());

        auto w = libmath::min(other.X() + X() + width(), other.X() + other.width()) - x;
        auto h = libmath::min(other.Y() + Y() + height(), other.Y() + other.height()) - y;

        return Bound(x, y, w, h);
    }
};

} // namespace libgraphic
