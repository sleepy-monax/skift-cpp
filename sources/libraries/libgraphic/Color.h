#pragma once

#include <libmath/Math.h>
#include <libruntime/Move.h>
#include <libruntime/Types.h>

namespace libgraphic
{

class __packed Color final
{
private:
    union {
        struct
        {
            uint8_t _red;
            uint8_t _green;
            uint8_t _blue;
            uint8_t _alpha;
        };

        uint32_t _packed;
    };

    Color(double red, double green, double blue, double alpha) : _red(red * 255),
                                                                 _green(green * 255),
                                                                 _blue(blue * 255),
                                                                 _alpha(alpha * 255)
    {
    }

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : _red(red),
                                                                     _green(green),
                                                                     _blue(blue),
                                                                     _alpha(alpha)
    {
    }

public:
    Color() : _red(0),
              _green(0),
              _blue(0),
              _alpha(0)
    {
    }

    uint8_t red() { return _red; }
    uint8_t green() { return _green; }
    uint8_t blue() { return _blue; }
    uint8_t alpha() { return _alpha; }

    u32 packed() { return _packed; }

    static Color RGB(uint8_t red, uint8_t green, uint8_t blue);

    static Color RGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    static Color HSV(double hue, double saturation, double value);

    static Color HSVA(double hue, double saturation, double value, double alpha);

    static Color alpha_blend(Color foreground, Color background);

    static Color lerp(Color from, Color to, double transition);

    ~Color() {}
};

} // namespace libgraphic
