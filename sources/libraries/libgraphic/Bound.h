#pragma once

#include <libgraphic/Point.h>

namespace libgraphic
{

class Bound
{
private:
    double _x;
    double _y;
    double _width;
    double _height;

public:
    double X() { return _x; }
    double Y() { return _y; }
    double width() { return _width; }
    double height() { return _height; }

    Point position() { return Point(_x, _y); }
    Point size() { return Point(_width, _height); }

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

    Bound(double x, double y, double width, double height) : _x(x),
                                                             _y(y),
                                                             _width(width),
                                                             _height(height)
    {
    }

    Bound(Point position, Point size) : _x(position.X()),
                                        _y(position.Y()),
                                        _width(size.X()),
                                        _height(size.Y)
    {
    }

    ~Bound() {}
};

} // namespace libgraphic
