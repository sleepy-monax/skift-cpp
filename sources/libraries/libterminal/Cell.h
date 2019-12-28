#pragma once

#include <libterminal/Attributes.h>
#include <libsystem/Unicode.h>

namespace libterminal
{

class Cell
{
private:
    Attributes _attributes;
    libsystem::Codepoint _codepoint;
    bool _dirty;

public:
    Attributes attributes() { return _attributes; }
    libsystem::Codepoint codepoint() { return _codepoint; }
    bool dirty() { return _dirty; }

    Cell() : _attributes(), _codepoint(U' '), _dirty(true) {}
    Cell(Attributes attributes, libsystem::Codepoint codepoint) : _attributes(attributes), _codepoint(codepoint), _dirty(true) {}

    Cell(Cell &other) : _attributes(other.attributes()), _codepoint(other.codepoint()), _dirty(other.dirty()) {}
    Cell(Cell &&other) : _attributes(other.attributes()), _codepoint(other.codepoint()), _dirty(other.dirty()) {}

    Cell &operator=(Cell &other)
    {
        _attributes = other.attributes();
        _codepoint = other.codepoint();
        _dirty = true;

        return *this;
    }

    Cell &operator=(Cell &&other)
    {
        _attributes = other.attributes();
        _codepoint = other.codepoint();
        _dirty = other.dirty();

        return *this;
    }

    bool operator==(Cell &other)
    {
        return _attributes == other.attributes() &&
               _codepoint == other.codepoint();
    }

    bool operator!=(Cell &other)
    {
        return !(*this == other);
    }
};

} // namespace libterminal
