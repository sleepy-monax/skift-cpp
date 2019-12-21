#pragma once

namespace libgraphic
{
class Size
{
private:
    int _width;
    int _height;

public:
    int width() { return _width; }
    int height() { return _height; }

    Size() : _width(0), _height(0) {}
    Size(int size) : _width(size), _height(size) {}
    Size(int width, int height) : _width(width), _height(height) {}

    ~Size() {}
};
} // namespace libgraphic
