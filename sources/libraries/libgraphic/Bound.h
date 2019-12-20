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
    inline double X() { return _x; }
    inline double Y() { return _y; }
    inline double width() { return _width; }
    inline double height() { return _height; }

    inline Point position() { return Point(_x, _y); }
    inline Point size() { return Point(_width, _height); }

    inline Point topleft() { return Point(_x, _y); }
    inline Point topright() { return Point(_x + _width, _y); }
    inline Point bottomleft() { return Point(_x, _y + _height); }
    inline Point bottomright() { return Point(_x + _width, _y + _height); }

    inline Bound(double x, double y, double width, double height) : _x(x),
                                                                    _y(y),
                                                                    _width(width),
                                                                    _height(height)
    {
    }

    inline Bound(Point position, Point size) : _x(position.X()),
                                               _y(position.Y()),
                                               _width(size.X()),
                                               _height(size.Y)
    {
    }

    inline ~Bound() {}
};

} // namespace libgraphic
