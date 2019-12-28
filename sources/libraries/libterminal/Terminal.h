#pragma once

/* Copyright © 2019-2020 N. Van Bossuyt.                                      */
/* This code is licensed under the 3-Clause BSD License.                      */
/* See: LICENSE.md                                                            */

#include <libterminal/Cell.h>
#include <libmath/MinMax.h>

namespace libterminal
{

#define TERMINAL_PARAMETERS_COUNT 8

enum class ParserState
{
    WAIT_FOR_ESC,
    ESCPECT_BRACKET,
    READ_ATTRIBUTE,
};

class Cursor
{
private:
    int _x;
    int _y;

public:
    int X() { return _x; }
    int Y() { return _y; }

    Cursor() : _x(0), _y(0) {}
    Cursor(int x, int y) : _x(x), _y(y) {}
};

struct Parameter
{
    int value;
    bool empty;
};

class Terminal : public libsystem::UTF8Stream
{
private:
    Cursor _cursor;
    Cursor _saved_cursor;

    int _width;
    int _height;
    Cell *_cells;

    Attributes _default_attributes;
    Attributes _current_attributes;

    Parameter _parameters[TERMINAL_PARAMETERS_COUNT];
    int _parameters_top;

    ParserState _state;

public:
    int width() { return _width; }

    int height() { return _height; }

    Cell cell_at(int x, int y)
    {
        return _cells[y * _width + x];
    };

    void set_cell(int x, int y, Cell cell)
    {
        if (x >= 0 && x < _width && y >= 0 && y < _height)
        {
            if (_cells[y * _width + x] != cell)
            {
                _cells[y * _width + x] = cell;

                on_cell_updated(x, y, cell);
            }
        }
    }

    Terminal(int width, int height)
        : _width(width),
          _height(height),
          _cells(new Cell[width * height]),
          _default_attributes(),
          _current_attributes(),
          _state(ParserState::WAIT_FOR_ESC)
    {
    }

    ~Terminal() {}

    void clear(int fromx, int fromy, int tox, int toy);

    void clear_line(int line);

    void set_cursor(int x, int y);

    void move_cursor(int vx, int vy);

    void scroll(int how_many_lines);

    void new_line();

    void backspace();

    void do_ansi(libsystem::Codepoint op, const Parameter *parameters);

    void append(libsystem::Codepoint codepoint);

    virtual libruntime::Error write_codepoint(libsystem::Codepoint codepoint);

    virtual void on_cell_updated(int x, int y, Cell cell) = 0;

    virtual void on_cursor_moved(Cursor cursor) = 0;
};

} // namespace libterminal