#pragma once

#include <libruntime/Move.h>
#include <libruntime/Types.h>

namespace libgraphic
{

class Color __packed final
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
    uint8_t red() { return _red; }
    uint8_t green() { return _green; }
    uint8_t blue() { return _blue; }
    uint8_t alpha() { return _alpha; }

    u32 packed() { return _packed; }

    static Color RGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        return Color(red, green, blue, 255);
    }

    static Color RGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    {
        return Color(red, green, blue, alpha);
    }

    static Color HSV(double hue, double saturation, double value) {}

    static Color HSVA(double hue, double saturation, double value, double alpha)
    {
        if (saturation == 0)
        {
            return Color(value, value, value, alpha);
        }

        if (hue == 1.0)
        {
            hue = 0;
        }

        hue *= 6;

        double f = hue - static_cast<int>(hue);

        double p = value * (1.0 - saturation);
        double q = value * (1.0 - (saturation * f));
        double t = value * (1.0 - (saturation * (1.0 - f)));

        switch (static_cast<int>(hue))
        {
        case 0:
            return Color(value, t, p, alpha);

        case 1:
            return Color(q, value, p, alpha);

        case 2:
            return Color(p, value, t, alpha);

        case 3:
            return Color(p, q, value, alpha);

        case 4:
            return Color(t, p, value, alpha);

        default:
            return Color(value, p, q, alpha);
        }
    }

    static Color alpha_blend(Color a, Color b);

    static Color lerp(Color a, Color b, double value);

    ~Color() {}
};

} // namespace libgraphic
