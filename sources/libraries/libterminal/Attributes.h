#pragma once

#include <libterminal/Color.h>

namespace libterminal
{

class Attributes
{
private:
    bool _bold;

    Color _foreground;
    Color _background;

public:
    bool bold() const { return _bold; }
    Color foreground() const { return _foreground; }
    Color background() const { return _background; }

    Attributes()
        : _bold(false),
          _foreground(Color::GREY),
          _background(Color::BLACK)
    {
    }

    Attributes(
        bool bold,
        Color foreground,
        Color background)
    {
        _bold = bold;
        _foreground = foreground;
        _background = background;
    }

    Attributes bolded() { return Attributes(true, _foreground, _background); }
    Attributes regular() { return Attributes(false, _foreground, _background); }
    Attributes with_foreground(Color foreground) { return Attributes(_bold, foreground, _background); }
    Attributes with_background(Color background) { return Attributes(_bold, _foreground, background); }

    bool operator==(const Attributes &other)
    {
        return _bold == other.bold() &&
               _foreground == other.foreground() &&
               _background == other.background();
    }

    bool operator!=(Attributes &other)
    {
        return !(*this == other);
    }
};

} // namespace libterminal
