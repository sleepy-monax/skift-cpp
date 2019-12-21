#pragma once

namespace libgraphic
{

class Point
{
private:
    int _x;
    int _y;

public:
    inline int X() const { return _x; }
    inline int Y() const { return _y; }

    inline static Point zero() { return Point(0, 0); }

    inline Point(int xy) : _x(xy), _y(xy) {}
    inline Point(int x, int y) : _x(x), _y(y) {}

    Point operator+(const Point &other)
    {
        return Point(X() + other.X(),
                     Y() + other.Y());
    }

    Point operator-(const Point &other)
    {
        return Point(X() - other.X(),
                     Y() - other.Y());
    }

    inline ~Point() {}
};

} // namespace libgraphic
