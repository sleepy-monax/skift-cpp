#include <libgraphic/Color.h>

namespace libgraphic
{

Color Color::RGB(uint8_t red, uint8_t green, uint8_t blue)
{
    return RGBA(red, green, blue, 255);
}

Color Color::RGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    return Color(red, green, blue, alpha);
}

Color Color::HSV(double hue, double saturation, double value)
{
    return HSVA(hue, saturation, value, 1.0);
}

Color Color::HSVA(double hue, double saturation, double value, double alpha)
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

// FIXME: Doesn't work if the background has alpha
Color Color::alpha_blend(Color foreground, Color background)
{
    uint alpha = foreground.alpha() + 1;
    uint inv_alpha = 256 - foreground.alpha();

    return Color::RGB((alpha * foreground.red() + inv_alpha * background.red()) / 256,
                      (alpha * foreground.green() + inv_alpha * background.green()) / 256,
                      (alpha * foreground.blue() + inv_alpha * background.blue()) / 256);
}

Color Color::lerp(Color from, Color to, double transition)
{
    return Color(libmath::lerp(from.red(), to.red(), transition),
                 libmath::lerp(from.green(), to.green(), transition),
                 libmath::lerp(from.blue(), to.blue(), transition),
                 libmath::lerp(from.alpha(), to.alpha(), transition));
}

Color Color::multiply(Color a, Color b)
{
    return Color(static_cast<uint8_t>(a.red() * b.red() / 255.0),
                 static_cast<uint8_t>(a.green() * b.green() / 255.0),
                 static_cast<uint8_t>(a.blue() * b.blue() / 255.0),
                 static_cast<uint8_t>(a.alpha() * b.alpha() / 255.0));
}

} // namespace libgraphic
